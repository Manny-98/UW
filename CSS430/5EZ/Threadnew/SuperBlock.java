public class SuperBlock
{

    // Instance variables
    public int totalBlocks;
    public int totalInodes;
    public int freeList;

    public SuperBlock(int diskSize)
    {
        // This is superblock represented in a byte array to represent the bytes of data
        byte[] block = new byte[Disk.blockSize];

        // Read it from disk since its first block in disk and put into super block
        SysLib.rawread(0, block);

        // 0 is block location
        totalBlocks = SysLib.bytes2int(block, 0);

        // 4 is the inode location
        totalInodes = SysLib.bytes2int(block, 4);

        // 8 is the free list location
        freeList = SysLib.bytes2int(block, 8);

        if(totalBlocks == diskSize && totalInodes > 0 && freeList >= 2)
        {
            return;
        }
        else
        {
            totalBlocks = diskSize;

            // Default inode block is 64
            format(64);
        }
    }


    public void sync()
    {
        byte[] data = new byte[Disk.blockSize];

        // Block location is 0
        SysLib.int2bytes(totalBlocks, data, 0);

        // Inode location is 4
        SysLib.int2bytes(totalInodes, data, 4);

        // Free list location is 8
        SysLib.int2bytes(freeList, data, 8);

        // Write all updated data back to disk
        SysLib.rawwrite(0, data);
    }


    public void format(int files)
    {
        // Check to see if there are less than 0 inodes (no files)
        if(files < 0)
        {
            // Set to default number
            files = 64;
        }

        // Total inodes is the inodes passed in
        totalInodes = files;

        // Go through all the inodes and initialize each node to defaults
        for(int i = 0; i < totalInodes; i++)
        {
            Inode node = new Inode();
            node.flag = 0;
            node.toDisk((short) i);
        }

        // Get the new head of free list
        freeList = 2 + (totalInodes/16);

        // Loop through the blocks and fill with empty
        for(int i = freeList; i < 999; i++)
        {
            byte[] data = new byte[Disk.blockSize];

            // Clear them by filling with 0
            for(int j = 0; j < Disk.blockSize; j++)
            {
                data[j] = 0;
            }

            SysLib.int2bytes(i+1, data, 0);
            SysLib.rawwrite(i, data);
        }

        byte[] data = new byte[Disk.blockSize];

        // Loop through and fill each element with 0
        for(int i = 0; i < Disk.blockSize; i++)
        {
            data[i] = 0;
        }

        // Last block is Null
        SysLib.int2bytes(-1, data, 0);
        SysLib.rawwrite(999, data);

        // Sync all data with updates
        sync();
    }


    public int getFreeBlock()
    {
        if(freeList > 0 && freeList < totalBlocks)
        {
            // Block number
            int blockNum = freeList;

            // Create byte array to hold data
            byte[] data = new byte[Disk.blockSize];

            SysLib.rawread(freeList, data);

            // Have to update the next free block
            freeList = SysLib.bytes2int(data, 0);

            SysLib.int2bytes(0, data, 0);

            // Updating current block
            SysLib.rawwrite(blockNum, data);

            // Return the location of the free block
            return  blockNum;
        }

        // Return invalid if no free block
        return -1;
    }


    public boolean returnBlock(int blockNumber)
    {
        // Check if out of range
        if(blockNumber < 0 || blockNumber > totalBlocks)
        {
            return false;
        }

        byte[] data = new byte[Disk.blockSize];

        // Put to end of freelist
        SysLib.int2bytes(freeList, data, 0);

        // Write to new block
        SysLib.rawwrite(blockNumber, data);

        freeList = blockNumber;

        return true;
    }

}
