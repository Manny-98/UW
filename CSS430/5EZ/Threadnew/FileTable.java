import java.util.*;
public class FileTable
{

    private Vector table;         // the actual entity of this file table
    private Directory dir;        // the root directory

    public FileTable( Directory directory )
    { // constructor
        table = new Vector( );     // instantiate a file (structure) table
        dir = directory;           // receive a reference to the Director
    }                             // from the file system

    public synchronized FileTableEntry falloc( String filename, String mode )
    {
        //initialize for later use
        Inode inode = null;
        short inodeNum = -1;

        //keep going until the desired inode is returned
        while(true)
        {
            // allocate a new file (structure) table entry for this file name
            if(filename.equals("/"))
            {
                inodeNum = (short) 0;
            }
            else {
                inodeNum = dir.namei(filename);
            }

            //if the requested inode is possible to exist
            if(!(inodeNum < 0))
            {
                inode = new Inode(inodeNum);

                //if file is in read mode
                if(mode.equalsIgnoreCase("r"))
                {
                    //if flag is set to write mode, wait until writing is done
                    if(inode.flag == (short)3)
                    {
                        try {
                            wait();
                        } catch(Exception e) {

                        }
                    } else {
                        //otherwise, change flag to read and exit loop
                        inode.flag = (short)2;
                        break;
                    }
                } else {
                    //if some other process is reading, wait for them to finish
                    if(inode.flag == (short)2)
                    {
                        try {
                            wait();
                        } catch(Exception e) {

                        }
                    } else {
                        //otherwise exit loop
                        break;
                    }
                }
            } else if(!(mode.equalsIgnoreCase("r"))) {
                //this case is for if there is a new file that needs to be added
                // allocate/retrieve and register the corresponding inode using dir
                inodeNum = dir.ialloc(filename);

                //new inode for new file
                inode = new Inode(inodeNum);
                inode.flag = (short)3;
                break;
            } else {
                //under correct circumstances, this case should not be reached
                return null;
            }
        }
        // increment this inode's count
        inode.count += 1;

        // immediately write back this inode to the disk
        inode.toDisk(inodeNum);

        //add new FileTableEntry into table
        FileTableEntry newFTE = new FileTableEntry(inode, inodeNum, mode);
        table.addElement(newFTE);

        // return a reference to this file (structure) table entry
        return newFTE;
    }

    public synchronized boolean ffree( FileTableEntry e )
    {
        // receive a file table entry reference
        Inode inode = new Inode(e.iNumber);

        //can only remove if that element is there to begin with
        if(table.remove(e))
        {
            //if the inode is trying to be read and the inode is available
            if(inode.flag == (short)2 && inode.count == (short)1)
            {
                //update to get rid of this inode
                notify();

                //change from read flag to used flag
                inode.flag = (short)1;
            } else if(inode.flag == (short)3) {
                //if entry's flag is write
                //change to used
                inode.flag = (short)1;

                //update all because contents may have changed
                notifyAll();
            }
            //got rid of one
            inode.count--;

            //save inode values to disk
            inode.toDisk(e.iNumber);

            //successfully found in table
            return true;
        }
        return false;
    }


    public synchronized boolean fempty( )
    {
        return table.isEmpty( );  // return if table is empty
    }                            // should be called before starting a format

}
