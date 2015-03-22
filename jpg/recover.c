/**
 * recover.c
 *
 * Luke Hodnett
 * Harvard CS50x
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

int main(int argc, char* argv[])
{
    // error check CLA input
    if (argc != 1)
    {
        printf("usage: ./recover.c\n");
        return 1;
    }
    
    // open RAW file
    FILE* file = fopen("card.raw", "r");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }
    
    // TODO search through file for jpg headers
        // TODO if header found
            // open a new ###.jpg file
            // write 512 bytes
            // scan next four bytes for new header
                // if not write 512 bytes again (maybe separate this into a separate function
                // if YES open new ###.jpg file and start again
            // end loop once all jpgs recovered (16 total)
}
