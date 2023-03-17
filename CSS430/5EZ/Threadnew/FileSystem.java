public class FileSystem {

    // Instance variables
    private SuperBlock superblock;
    private Directory directory;
    private FileTable filetable;

    private final int SEEK_SET = 0;
    private final int SEEK_CUR = 1;
    private final int SEEK_END = 2;

    public FileSystem(int diskBlocks)
    {
        // Instantiate members
        superblock = new SuperBlock(diskBlocks);
        directory = new Directory(superblock.totalInodes);
        filetable = new FileTable(directory);

        // Root directory from disk
        FileTableEntry dirEnt = open("/", "r");
        int dirSize = fsize(dirEnt);

        if(dirSize > 0)
        {
            byte[] dirData = new byte[dirSize];
            read(dirEnt, dirData);
            directory.bytes2directory(dirData);
        }
        close(dirEnt);
    }

    
     public boolean format(int files)
     {
        while (!filetable.fempty()) {}
        // Create amount of files in superblock
        superblock.format(files);
        // Instantiate directory and filetable with default
        directory = new Directory(superblock.totalInodes);
        
        filetable = new FileTable(directory);
        
        return true;
      }

    public FileTableEntry open(String filename, String mode) {
        FileTableEntry ftEnt = filetable.falloc(filename, mode);
        if (ftEnt == null) {
            return null;
        }
        if (mode.compareTo("w") == 0) {
            if (this.deallocAllBlocks(ftEnt) == false) {
                return null;
            }
        }
        return ftEnt;
    }


    public boolean close(FileTableEntry ftEnt)
    {
        // Check to see if entry exists
        if(ftEnt == null)
        {
            return false;
        }

        synchronized(ftEnt)
        {
            // Decrement count of users
            ftEnt.count--;

            // Check if no users
            if(ftEnt.count <= 0)
            {
                return filetable.ffree(ftEnt);
            }

            return true;
        }
    }


    public synchronized int fsize(FileTableEntry ftEnt)
    {
        // Check to see if exists
        if(ftEnt == null)
        {
            return -1;
        }

        synchronized(ftEnt)
        {
            return ftEnt.inode.length;
        }
    }
// This function reads bytes from a file represented by a 
//FileTableEntry object and stores them in a buffer.
    public int read(FileTableEntry ftEnt, byte[] buffer) {
        // Check if the file is opened in write or append mode, if so, return an error.
        if (ftEnt.mode.compareTo("w") == 0 || ftEnt.mode.compareTo("a") == 0) {
            return -1;
        }
        int bytes = 0;
        int length = buffer.length;
        synchronized (ftEnt) {
            while (length > 0 && ftEnt.seekPtr < fsize(ftEnt)) {
                int blockNum = ftEnt.inode.findTargetBlock(ftEnt.seekPtr);
                if (blockNum == -1) {
                    break;
                }
                byte[] block = new byte[Disk.blockSize];
                SysLib.rawread(blockNum, block);
                // Calculate the offset into the block where the seek pointer is located.
                int offset = ftEnt.seekPtr % Disk.blockSize;
                // Calculate the maximum number of bytes that can be read from the block.
                int read = Math.min(Math.min(Disk.blockSize - offset, length), fsize(ftEnt) - ftEnt.seekPtr);
                System.arraycopy(block, offset, buffer, bytes, read);
                // Update the seek pointer and the number of bytes read.
                ftEnt.seekPtr += read;
                bytes += read;
                length -= read;
            }
            return bytes;
        }
    }

    public int write(FileTableEntry ftEnt, byte[] buffer)
    {
        // Check if entry and buffer exist as well as the mode
        if(ftEnt == null || ftEnt.mode.equals("r"))
        {
            return -1;
        }

        int restToWrite = 0;
        int size = buffer.length;

        synchronized(ftEnt)
        {
            // Keep looping through as long as something is in the buffer
            while(size > 0)
            {
                // Find the seek ptr
                int writeLoc = ftEnt.inode.findTargetBlock(ftEnt.seekPtr);

                if(writeLoc == -1)
                {
                    // Write to the next free block
                    short newLoc = (short) superblock.getFreeBlock();
                    int tempPtr = ftEnt.inode.registerTargetBlock(ftEnt.seekPtr, newLoc);

                    // Check if indirect block is initialized
                    if(tempPtr == -3)
                    {
                        short freeBlock = (short) superblock.getFreeBlock();

                        // Check if empty
                        if(!ftEnt.inode.registerIndexBlock(freeBlock))
                        {
                            return -1;
                        }

                        // Check if the pointer is valid
                        if(ftEnt.inode.registerTargetBlock(ftEnt.seekPtr, newLoc) != 0)
                        {
                            return -1;
                        }
                    }
                    // Check if direct block has error
                    else if(tempPtr == -2 || tempPtr == -1)
                    {
                        return -1;
                    }

                    writeLoc = newLoc;
                }

                // Create new byte array to store data at write location and write the data
                byte[] data = new byte[512];
                SysLib.rawread(writeLoc, data);

                int nextPtr = ftEnt.seekPtr % 512;
                int index = 512 - nextPtr;

                // Check to see if index is larger than the size, if so we found it
                if(index > size)
                {
                    //Fill array and write data
                    System.arraycopy(buffer, restToWrite, data, nextPtr, size);
                    SysLib.rawwrite(writeLoc, data);

                    // Increment the seekptr and the remaining bytes to write by size
                    ftEnt.seekPtr += size;
                    restToWrite += size;
                    size = 0;
                }
                else
                {
                    // Fill array and write data
                    System.arraycopy(buffer, restToWrite, data, nextPtr, index);
                    SysLib.rawwrite(writeLoc, data);

                    // Increment the seekptr and remaining bytes to write by index
                    ftEnt.seekPtr += index;
                    restToWrite += index;
                    size -= index;
                }
            }

            // Check if the seekptr is larger than file
            if(ftEnt.seekPtr > ftEnt.inode.length)
            {
                // Update inode size to where seekptr is
                ftEnt.inode.length = ftEnt.seekPtr;
            }

            // Write data of specified inode to disk
            ftEnt.inode.toDisk(ftEnt.iNumber);

            return restToWrite;
        }
    }


    private boolean deallocAllBlocks(FileTableEntry ftEnt)
    {
        // Value for invalid number
        short invalid = -1;

        // Check if entry is null pointer
        if(ftEnt.inode.count != 1)
        {
            return false;
        }

        // Loop through direct blocks
        for(short i = 0; i < 11; i++)
        {
            // If not invalid, clears all direct blocks
            if(ftEnt.inode.direct[i] != invalid)
            {
                superblock.returnBlock(i);
                ftEnt.inode.direct[i] = invalid;
            }
        }

        // Create byte array to hold indirect block data
        byte[] data = ftEnt.inode.unregisterIndexBlock();

        // Check if the array of indirect block data exists
        if(data != null)
        {
            short blockNum;

            // Loop through the and see if the block number equals an invalid pointer
            while((blockNum = SysLib.bytes2short(data, 0)) != invalid)
            {
                // Return the block
                superblock.returnBlock(blockNum);
            }
        }

        // Write the filetable entries to disk
        ftEnt.inode.toDisk(ftEnt.iNumber);

        return true;
    }


    public boolean delete(String filename)
    {
        // Get the entry for the file
        FileTableEntry fte = open(filename, "w");

        // Check and free block to delete
        if(directory.ifree(fte.iNumber) && close(fte))
        {
            return true;
        }

        return false;
    }


    public int seek(FileTableEntry ftEnt, int offset, int whence)
    {
        synchronized(ftEnt)
        {
            // Whence is the location of the seekptr, so depends on it
            switch(whence)
            {
                // Case for beginning of file or SEEK_SET
                case SEEK_SET:
                    ftEnt.seekPtr = offset;
                    break;

                // Case for the current position in file or SEEK_CUR
                case SEEK_CUR:
                    ftEnt.seekPtr += offset;
                    break;

                // Case for the end of file or SEEK_END
                case SEEK_END:
                    ftEnt.seekPtr = ftEnt.inode.length + offset;
                    break;

                // In any other case
                default:
                    return -1;
            }

            // Check if seek pointer is less than SEEK_SET
            if(ftEnt.seekPtr < 0)
            {
                // Start it at SEEK_SET
                ftEnt.seekPtr = 0;
            }

            // Check if seek pointer is greater than inode length
            if(ftEnt.seekPtr > ftEnt.inode.length)
            {
                ftEnt.seekPtr = ftEnt.inode.length;
            }

            return ftEnt.seekPtr;
        }
    }


}
