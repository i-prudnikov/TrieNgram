#include "trie.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <random>

using std::cout;
using std::endl;
using std::runtime_error;

using std::string;
using std::vector;

//TODO: make using for cout & endl

//Forward declaration of utilities
//Utility for download files using curl
bool DownloadCorpus(std::string storageFolder,std::string urlStorage);
//Popen utility
bool exec(UnicodeString cmd);
//-----------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    using namespace std::chrono;
    //Initialize random generator

    {
    Trie tt;
    tt.chainRank=3;

    auto t1 = high_resolution_clock::now();

        tt.BuildTrieFromFile("./Corpus/T1.txt");
        tt.BuildTrieFromFile("./Corpus/T2.txt");
        tt.BuildTrieFromFile("./Corpus/T3.txt");
        //tt.BuildTrieFromFile("./Corpus/T4.txt");
        //tt.BuildTrieFromFile("./Corpus/T5.txt");
        //tt.BuildTrieFromFile("./Corpus/T6.txt");
        //tt.BuildTrieFromFile("./Corpus/T7.txt");
        //tt.BuildTrieFromFile("./Corpus/T8.txt");
        //tt.BuildTrieFromFile("./Corpus/corpus_0.txt");

        cout<<"Most freq:"<<endl;
        tt.CompleteText(UnicodeString("анна павловна соображая"),100);

        cout<<"Prob distrib:"<<endl;
        tt.CompleteText(UnicodeString("анна павловна соображая"),100,Trie::Mode::ProbDistrib);
    auto t2 = high_resolution_clock::now();

//    deque<UnicodeString> Ngram1{"word1","word2","word3"};
//    deque<UnicodeString> Ngram2{"word1","word2","word31"};
//    deque<UnicodeString> Ngram3{"word4","word5"};
//    deque<UnicodeString> Ngram4{"word6","word7"};
//    deque<UnicodeString> Ngram5{"word6","word8"};
//    tt.InsertNGram(Ngram1);
//    tt.InsertNGram(Ngram2);
//    tt.InsertNGram(Ngram3);
//    tt.InsertNGram(Ngram4);
//    tt.InsertNGram(Ngram5);

//    int nc=0;
//    for (auto it = tt.begin(); it != tt.end(); ++it)
//        {
//            //cout<<*it<<endl;
//            nc++;
//        }
//
//    cout<<"nc = "<<nc<<", tt.nodesCount = "<<tt.nodesCount<<endl;

    //cout<<endl<<"BuildTrieFromFile() new takes: "<<duration_cast<nanoseconds>( t2 - t1 ).count()/1000000<<" milliseconds"<<endl<<endl;

    }
    cout <<"Done!";



    /*
    std::string usage ="==========================\nUsage of this tool: You must provide the following options:\n\
\"-m mode\", where mode describes the logic that will be applied, possible values:\n\
 \"build\" - means that Markov Chain will be built and saved to output dump file \n\
 \"exploit\" - means that previously built and dumped Markov Chain will be loaded from dump file and provided text (specified with -t option) will be completed.\n------------\n\
\"-f filename\", where filename stores a list of URLs (one URL per line)\n\
that will be used to download the corpus. These files will be used to built Markov chain. Valid only for \"build\", ignored for exploit mode.\n------------\n\
\"-n digit\", where digit is an integer from 0. This specifies the rank of Markov chain. Valid only for \"build\", ignored for exploit mode.\n------------\n\
\"-d filename\", where filename is the name of the output file to dump the Markov chain.\n------------\n\
\"-t text to complete\", specify here the text to complete in quotes (\"text\"). Valid only for \"exploit\" mode, for build mode - ignored.\n------------\n\
\"-l digit\",where digit is an integer from  1. This specifies how many words to build based on the text provided in -t option. Valid only for \"exploit\" mode for build mode - ignored.\
\n==========================\n";

    int nRank=-1, wordsCountToBuild=0;
    std::string urlStorage="", dumpFile="", folderToStore="./Corpus", mode="";
    UnicodeString textToComplete="";
    int opt;
    opterr=0;

    //Initialize random generator
    std::srand(std::time(0));

    //Primary object;
    //Завернуть в unique_ptr (изучить главу 15 сначала)
    // А нужен ли указатель здесь??? Можно же создать объект...
    // А можно и завернуть в unique_ptr
    Trie *tt = new Trie;

    try
    {
        //if(argc<9){throw std::runtime_error(usage);}
        while((opt = getopt(argc, argv, ":m:n:d:f:l:t:")) != -1)
            {
            switch(opt)
                {
                    case 'm':
                        mode = std::string((char*)optarg);
                        if (mode!="build" && mode!="exploit") {std::cout<<"=> -m option value "<<mode<<" is not valid..."<<std::endl;throw std::runtime_error(usage);}
                        std::cout<<"=> Mode is: "<<mode<<std::endl;
                        break;
                    case 'n':
                        //nRank = std::string(opt);
                        std::istringstream((char*)optarg)>>nRank; ///What the fuck???
                        if(nRank<0){std::cout<<"=> -n option value is not valid..."<<std::endl;throw std::runtime_error(usage);}
                        std::cout<<"=> Rank of Markov chain is: "<<nRank<<std::endl;
                        break;
                    case 'f':
                        urlStorage = std::string((char *)optarg);
                        if (urlStorage.find_first_of("\"*?;:~<>\\") != std::string::npos) {std::cout<<"=> -f option value "<<urlStorage<<" is not valid..."<<std::endl;throw std::runtime_error(usage);}
                        std::cout<<"=> URL File name is: "<<urlStorage<<std::endl;
                        break;
                    break;
                    case 'd':
                        dumpFile = std::string((char *)optarg);
                        if (dumpFile.find_first_of("\"*?;:~<>\\") != std::string::npos) {std::cout<<"=> -d option value "<<dumpFile<<" is not valid..."<<std::endl;throw std::runtime_error(usage);}
                        std::cout<<"=> Dump file name is: "<<dumpFile<<std::endl;
                        break;
                    break;
                    case 't':
                        textToComplete = UnicodeString(optarg);
                        std::cout<<"=> Text to complete is: "<<textToComplete<<std::endl;
                        break;
                    case 'l':
                        std::istringstream((char*)optarg)>>wordsCountToBuild;
                        if(wordsCountToBuild<1){std::cout<<"=> -l option value is not valid..."<<std::endl;throw std::runtime_error(usage);}
                        std::cout<<"=> Count of words to be built based on provide text is: "<<wordsCountToBuild<<std::endl;
                        break;
                    case ':':
                        std::cout<<"One of the options provided without value..."<<std::endl;
                        throw std::runtime_error(usage);
                    break;
                    case '?':
                        std::cout<<"Unknown option \""<<char(optopt)<<"\"..."<<std::endl;
                        throw std::runtime_error(usage);
                    break;
                    default:
                        throw std::runtime_error(usage);
                    break;
                  }
            }
    if (mode =="" || dumpFile=="" ){std::cout<<"=> One of the mandatory option is not provided..."<<std::endl;throw std::runtime_error(usage);}

    std::cout<<std::endl;
    std::cout<<"Start processing...."<<std::endl;

    if (mode == "build")
    {
        //! ДОбавить сюда объявление объекта ???
        if (urlStorage == ""){std::cout<<"=> -f option is not provided..."<<std::endl;throw std::runtime_error(usage);}
        if (nRank==-1){std::cout<<"=> -n option is not provided..."<<std::endl;throw std::runtime_error(usage);}

        //Starting from here it looks that everything is ready to start working...
        //Downloading the corpus using curl
        if(!DownloadCorpus(folderToStore,urlStorage)){throw std::runtime_error("Error downloading corpus... ");}

        //Building Markov Chain
        if(!tt->BuildTrieFromFolder(nRank+2,folderToStore)){throw std::runtime_error("Can't build Markov chain from corpus... ");}

        // Saving chain to file
        if(!tt->SerializeTree(dumpFile)){throw std::runtime_error("Can't save Markov chain to file... ");}
    }
    else if (mode == "exploit")
    {
        //! ДОбавить сюда объявление объекта ???
        if (wordsCountToBuild == 0){std::cout<<"=> -l option is not provided..."<<std::endl;throw std::runtime_error(usage);}
        if (textToComplete == ""){std::cout<<"=> -t option is not provided..."<<std::endl;throw std::runtime_error(usage);}

        if(!tt->DeleteTree()){throw std::runtime_error("Can't delete trie tree... ");}

        if(!tt->DeserializeTree(dumpFile)){throw std::runtime_error("Can't load Markov chain from file... ");}

        std::cout <<"Trying to complete text with most frequent approach: "<<"\""<<textToComplete<<"\""<<std::endl;
        if(!tt->CompleteText(&textToComplete,wordsCountToBuild)){throw std::runtime_error("Error during text completion... ");}
        std::cout<<std::endl;

        std::cout <<"Trying to complete text with probability distribution approach: "<<"\""<<textToComplete<<"\""<<std::endl;
        if(!tt->CompleteTextWithProbabilityDistrib(&textToComplete,wordsCountToBuild)){throw std::runtime_error("Error during text completion... ");}
        std::cout<<std::endl;
    }

    delete tt;
    return 0;

    }
    catch(const std::exception &r)
    {
      if (tt){delete tt;}
      std::cout<<r.what()<<std::endl;
      return 1;
    }
    */
}
//---------------------------------------------------------------------------------------------------------
bool exec(UnicodeString cmd)
{

    int length = cmd.length();
    UErrorCode status = U_ZERO_ERROR;
    char * cmdLine = NULL;
    FILE* pipe = NULL;
    try{

        cmdLine = new char[length+1];

        cmd.extract(cmdLine,length,NULL,status);
        if (U_FAILURE(status)) {throw std::runtime_error("Failed to convert to char...");}

        cmdLine[length]='\0';

       pipe = popen(cmdLine, "r");
        if (!pipe) {throw std::runtime_error("Failed to start process with popen!"); }

        delete [] cmdLine;
        pclose(pipe);
        return true;
      }
    catch(const std::exception &r)
    {
      std::cout<<r.what()<<std::endl;
      if (cmdLine){delete [] cmdLine;}
      if (pipe){pclose(pipe);}
      return false;
    }
}
//---------------------------------------------------------------------------------------------------------
bool DownloadCorpus(std::string storageFolder,std::string urlStorage)
{
    long buffsize=1000;
    UFILE* f = NULL;
    int iCount = 0;
    //Assume that it will be not more than 1000 symbols per url
    UChar buffer[1000];
    UnicodeString url,cmdline("curl --progress-bar -o "),fileName = UnicodeString(storageFolder.data()) +"/corpus_";
    DIR *pDir = NULL;

    std::cout<<"==>Downloaded files will be stored in "<<storageFolder<<" folder..."<<std::endl;

    try{
        // open a UTF-8 file for reading
        f = u_fopen(urlStorage.data(), "r", NULL, "UTF-8");
        if(!f){throw std::runtime_error("Error: can't open file...");}


        //Creating a directory to store downloaded files...
        pDir = opendir(storageFolder.data());
        if(!pDir){
            if(mkdir(storageFolder.data(),S_IRUSR|S_IWUSR|S_IXUSR)!=0){throw std::runtime_error("Can't create directory for storing curl downloaded files...");}
        }

        while(!u_feof(f))
        {
            if(!u_fgets(buffer,buffsize-1,f)){throw std::runtime_error("Failed to read data from file...");}
            url = ((UnicodeString)buffer).trim();if (url==""){continue;}

            std::cout<<"==>URL parsed: "<<url<<std::endl;

            //This kind of cmd line will be used
            //curl --progress-bar -o filename.txt http://linguis.ru/static/tolstoy.txt
            fileName += UnicodeString(std::to_string((long long int)iCount).data())+".txt";
            cmdline+=fileName+" "+"\""+url+"\"";
            std::cout<<"==>curl cmd. line: "<<cmdline<<std::endl;

            if(!exec(cmdline)){throw std::runtime_error("Failed to start curl...");}

            iCount++;
        }

        // close the file resource
        if(pDir){closedir(pDir);}
        u_fclose(f);
        return true;
    }
    catch(const std::exception &r)
    {
      std::cout<<r.what()<<std::endl;
      if(pDir){closedir(pDir);}
      if(f){u_fclose(f);}
      return false;
    }

}

