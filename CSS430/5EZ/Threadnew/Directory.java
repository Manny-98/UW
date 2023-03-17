public class Directory
{

    // Instance variables
    private static int maxChars = 30; // max characters of each file name

    // Directory entries
    private int fsize[];        // each element stores a different file size.
    private char fnames[][];    // each element stores a different file name.
    private int size;

    public Directory(int maxInumber)
    {
        fsize = new int[maxInumber];     // maxInumber = max files

        for ( int i = 0; i < maxInumber; i++ )
        {
            fsize[i] = 0;                 // all file size initialized to 0
        }

        size = maxInumber;
        fnames = new char[maxInumber][maxChars];
        String root = "/";                // entry(inode) 0 is "/"
        fsize[0] = root.length( );        // fsize[0] is the size of "/".
        root.getChars( 0, fsize[0], fnames[0], 0 ); // fnames[0] includes "/"
    }

    public int bytes2directory(byte data[])
    {
        // Check if there is anything in byte array and return invalid if not
        if(data == null || data.length == 0)
        {
            return -1;
        }

        int diffBits = 0;

        // Loop through the fsize and set bytes to int, incrementing by 4
        for(int i = 0; i < size; i++)
        {
            fsize[i] = SysLib.bytes2int(data, diffBits);
            diffBits += 4;
        }

        // Loop through fnames
        for(int i = 0; i < size; i++)
        {
            // Max bytes are 30 chars which is 60 bytes
            String name = new String(data, diffBits, maxChars * 2);
            name.getChars(0, fsize[i], fnames[i], 0);
            diffBits += maxChars * 2;
        }

        return 1;
    }


    public byte[] directory2bytes()
    {
        // Create directory of correct size
        byte[] directory = new byte[64 * size];

        int diffBits = 0;

        // Loop through the length of directory of file sizes
        for(int i = 0; i < size; i++)
        {
            // Insert data into the byte array
            SysLib.int2bytes(fsize[i], directory, diffBits);
            diffBits += 4;
        }

        // Loop through the file names
        for(int i = 0; i < size; i++)
        {
            String name = new String(fnames[i], 0, fsize[i]);
            byte[] data = name.getBytes();
            System.arraycopy(data, 0, directory, diffBits, data.length);
            diffBits += maxChars * 2;
        }

        return directory;
    }

    public short ialloc(String filename)
    {

        // Loop through the lengths of file sizes array
        for(int i = 0; i < size; i++)
        {
            // Check if the size of file is 0
            if(fsize[i] == 0)
            {
                // Find the smaller name
                fsize[i] = Math.min(filename.length(), maxChars);
                filename.getChars(0, fsize[i], fnames[i], 0);

                return (short) i;
            }
        }

        return (short) -1;
    }

 
    public boolean ifree(short iNumber)
    {
		
		// If valid free the file
		if(fsize[iNumber] > 0 && iNumber < 30)
		{
			fsize[iNumber] = 0;
			
			return true;
		}
		else
		{
			return false;
		}
    }


    public short namei(String filename)
    {

        // Loop through the directory
        for(int i = 0; i < size; i++)
        {
            // If the element we are comparing has same length as the one we search for
            if(fsize[i] == filename.length())
            {
                String name = new String(fnames[i], 0, fsize[i]);
				
                if(filename.equals(name))
                {
                    return (short) i;
                }
            }
        }

        return (short) -1;
    }

}
