#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <string.h> 
#include <sstream> 
#include <vector>

#include "ManifestParser.h"
#include "F4mDownloader.h"
#include "console.h"
#include "debug.h"

using namespace f4m;

////////////////////////////////////////////////////////
// GLOBALS
///////////////////////////////////////////////////////
CF4mDownloader g_downloader;

void SigHandler (int signum)
{
   // Ignore it so that another Ctrl-C doesn't appear any soon
   signal(signum, SIG_IGN); 
   
   ConsoleAppContainer::getInstance().terminate();
   g_downloader.terminate();
}

int main(int argc, char *argv[])
{
    //
    ConsoleAppContainer &console = ConsoleAppContainer::getInstance();
    signal(SIGINT, SigHandler);
    
    if(1 == argc)
    {
        fprintf(stderr, "F4MDump v0.1\n");
        fprintf(stderr, "(c) 2014 samsamsam@o2.pl\n");
        return 0;
    }
    
    // get parameters from cmdline 
    if(3 > argc || 5 < argc)
    {
        printDBG("Wrong arguments\n");
        return -1;
    }
    
    std::string wget(argv[1]);
    std::string manifestUrl(argv[2]);
    
    if(3 == argc)
    {
        try
        {
            printDBG("Qualities report only\n");
            std::stringstream cmd;
            cmd << "{ \"qualities\":[";
            CManifestParser parser(wget);
            parser.parseManifest(manifestUrl);
            std::vector<int32_t> allBitrates = parser.getAllBitrates();
            for(uint32_t i=0; i < allBitrates.size(); ++i)
            {
                cmd << allBitrates[i];
                if(i < allBitrates.size()-1)
                {
                    cmd << ", ";
                }
            }
            cmd << "] }";
            fprintf(stderr, "%s\n", cmd.str().c_str());
        }
        catch(const char *err)
        {
            fprintf(stderr, "%s\n", err);
            return -1;
        }
        return 0;
    }
    
    std::string outFile(argv[3]);
    std::string tmpFile("");
    int32_t bitrate = 0;
    if(5 == argc)
    {
        bitrate = atoi(argv[4]);
    }
    
    // download
    try
    {
        g_downloader.download(wget, manifestUrl, outFile, tmpFile, bitrate);
    }
    catch(const char *err)
    {
        fprintf(stderr, "%s\n", err);
        return -1;
    }

    
    return 0;
}