#include "trie.h"
//-----------------------------------------------------------------------------------------------------------------------
Trie::Trie(int nRank) : chainRank(nRank),rootNode(Node(UnicodeString(""),shared_ptr<NodeElement>(new NodeElement(0))))
{
    lastNode = &rootNode;
    rootNode.second->parent = nullptr;
    nodesCount = 1;
}
//-----------------------------------------------------------------------------------------------------------------------
Trie::Trie(const Trie & t) : rootNode(Node(UnicodeString(""),shared_ptr<NodeElement>(new NodeElement(0)))) //root node is always "clear" node - it is a starting point only;
{
   chainRank = t.chainRank;
   lastNode = &rootNode;
   rootNode.second->parent = nullptr;
   nodesCount = 1;

   //Copying the tree:
   for (auto keyValuePair : t.rootNode.second->childNodes)
        CopyNodesRecursive(keyValuePair,rootNode);
}
//-----------------------------------------------------------------------------------------------------------------------
Trie & Trie::operator=(const Trie & t)
{
    if (&t!=this)
    {
      //Clear current tree
      for (auto & keyValuePair : rootNode.second->childNodes)
            DeleteNode(keyValuePair);

      if(rootNode.second->childNodes.size()!=0)
            rootNode.second->childNodes.clear();
      lastNode = &rootNode;
      nodesCount = 1;

      //Copy tree
      chainRank = t.chainRank;

      //Copy tree
      for (auto & keyValuePair : t.rootNode.second->childNodes)
         CopyNodesRecursive(keyValuePair,rootNode);
    }
  return *this;
}
//-----------------------------------------------------------------------------------------------------------------------
ostream & operator<<(ostream & os, const Trie & t)
{
 int totalNodes=0;
 os<<"rank = "<<t.chainRank<<", nodes = "<<totalNodes<<endl;
 os<<"root"<<endl;
 //Print nodes without recursion

 for (const auto & keyValuePair : t.rootNode.second->childNodes)
    os<<"\t"<<keyValuePair.first<<":"<< keyValuePair.second->count<<endl;

// for (const Trie::Node *nd = t.Begin(&t.rootNode);nd;nd=t.Next())
//	os<<"\t"<<nd->first<<":"<< nd->second->count<<endl;

 return os;
}
//-----------------------------------------------------------------------------------------------------------------------
ostream & operator<<(ostream & os, const Trie::Node & n)
{
 os<<"Node:"<<n.first<<" ,count:"<<n.second->count<<" ,child:"<<n.second->childNodes.size()<<endl;
 return os;
}
//-----------------------------------------------------------------------------------------------------------------------
Trie::~Trie()
{
   for (auto & keyValuePair : rootNode.second->childNodes)
        DeleteNode(keyValuePair);

   if(rootNode.second->childNodes.size()!=0)
        rootNode.second->childNodes.clear();
}
//-----------------------------------------------------------------------------------------------------------------------
void Trie::DeleteNode(Node & nd)
{
    if (nd.second->childNodes.size() == 0)
        return;
    else
    {
        for (auto & keyValuePair : nd.second->childNodes)
            DeleteNode(keyValuePair);
    }
    try{nd.second->childNodes.clear();}
    catch(exception &e)
    {throw runtime_error(string(__FUNCTION__)+": Fatal error - can't delete node from tree. Exception content:"+e.what());}

    return;
}
//-----------------------------------------------------------------------------------------------------------------------
void Trie::CopyNodesRecursive(const Node & sourceNode,Node & parent)
{
    try{
        auto res = parent.second->childNodes.insert(Node(sourceNode.first,shared_ptr<NodeElement>(new NodeElement(sourceNode.second->count))));
        if (!res.second)
            throw runtime_error("Insert method failed...");
            res.first->second->parent = &parent;
        for (auto & sourceNodeChild : sourceNode.second->childNodes)
            CopyNodesRecursive(sourceNodeChild,*res.first);

        lastNode = &*res.first;
        return;
    }
    catch (exception & e)
    {throw runtime_error(string(__FUNCTION__)+": Fatal error: can't insert node into tree. Reason:"+e.what());}
}
//-----------------------------------------------------------------------------------------------------------------------
bool Trie::InsertNGram(const deque<UnicodeString> & nGram)
{
    Node * currNode = &rootNode;
    Container::iterator tmpNode;

    if(nGram.size()==0){cout<<"nGram is empty..."<<endl;return false;}

    try
    {
        for (auto word : nGram)
        {
            // create a new node if path doesn't exists
            tmpNode = currNode->second->childNodes.find(word);

            if (tmpNode == currNode->second->childNodes.end()) //Node is not exist
                {
                   auto res = currNode->second->childNodes.insert(Node(word,shared_ptr<NodeElement>(new NodeElement)));
                   if (!res.second) throw runtime_error("Insert method failed...");
                   nodesCount++;
                   tmpNode = res.first;
                   tmpNode->second->parent = currNode;
                   lastNode = &*tmpNode;
                }
            else // The node is already in the trie, increase the node count
                {tmpNode->second->count +=1;}

            // go to the next node // Don't like this as it can be not safe...

            currNode = &*tmpNode;
        }

        return true;
  }

  catch(const exception &e)
  {
   cout<<__FUNCTION__<<": Fatal error - can't insert node into tree. Reason:"<<e.what()<<endl;
   return false;
  }
}
//-----------------------------------------------------------------------------------------------------------------------
Trie::Node * Trie::SearchNode(const deque<UnicodeString> & nodePath)
{
    //For return value it should be "boost::optional<Trie::Node&>()" - it will be part of C++17 standard with std::optional.

    Node* currNode = &rootNode;
    Container::iterator tmpNode;

    try
    {
     if(nodePath.size()==0)
        throw logic_error(string(__FUNCTION__)+": nodePath is empty...");

     for(auto word : nodePath)
        {
            // go to next node
//            cout<<word<<endl;
            tmpNode = currNode->second->childNodes.find(word);
            if (tmpNode != currNode->second->childNodes.end())
                {
 //               cout<<"k:"<<tmpNode->first<<endl;
                currNode = &*tmpNode;
//                cout<<"cn_k:"<<currNode->first<<endl;
//                cout<<"chld:"<<currNode->second->childNodes.size()<<endl;
//                for (auto elem: currNode->second->childNodes)
//                    cout<<"\t"<<elem.first<<":"<<elem.second->count<<endl;
                }
            else//reach end of path in Trie
                return nullptr;
        }
    }
	catch(logic_error &e)
	{
        cout<<e.what()<<endl;
        return nullptr;
	}

	return currNode;
}
//-----------------------------------------------------------------------------------------------------------------------
UnicodeString Trie::GetMostFrequentChildNode(const deque<UnicodeString> & nodePath)
{

  Node * currNode = SearchNode(nodePath);
  if (!currNode){return "Node is not found";}
  Container childNodes = currNode->second->childNodes;
  UnicodeString currKey;
  unsigned short currUiCount=0;

  if (childNodes.size()==0){return "Node child not found";}
  //if node has only one child node then just return the first child node name
  else if (childNodes.size()==1){return childNodes.begin()->first;}
  else // node has more than one child, simply return the one that has max uiCount (most frequent)
  {
    //This is actually a trade-off between using unordered_map vs other ordered containers (getting the most frequent element be comparing uiCount)...
    //unordered_map shows better performance for the find operation...that is crucial as we have a lot of operation for finding keys...
    //Can be also a possible option to copy contents of child nodes to any ordered container swapping key and value... then get either top or bottom element
    for (const auto & keyValuePair : childNodes)
        if (currUiCount < keyValuePair.second->count)// In case we have one or more nodes with the same uiCount we
                                                               // just pick the first which returned by iterator as we assume
                                                               // the same probability for every node
        {
            currUiCount =  keyValuePair.second->count;
            currKey = keyValuePair.first;
        }
  }

 return currKey;
}
//-----------------------------------------------------------------------------------------------------------------------
UnicodeString Trie::GetRandomChildNode(const deque<UnicodeString> & nodePath)
{
  Node* currNode = SearchNode(nodePath);
  if (!currNode){return UnicodeString("Node is not found");}

  Container & childNodes = currNode->second->childNodes;   //Just to make things simpler to read
  UnicodeString key;
  //
  vector<int> vFreq;
  vector<UnicodeString> vKey;

  if (childNodes.size()==0){return "Node child not found";}
  //if node has only one child node then just return the first child node name
  else if (childNodes.size()==1){return childNodes.begin()->first;}
  else // node has more than one child, simply return the one that has max uiCount (most frequent)
  {
      for (auto & keyValuePair : childNodes)
        {
         vFreq.push_back(keyValuePair.second->count);
         vKey.push_back(keyValuePair.first);
        }

      std::random_device rd;
      std::mt19937 gen(rd());
      std::discrete_distribution<> disc_dist(vFreq.begin(),vFreq.end());

      key = vKey.at(disc_dist(gen));
  }
  return key;
 }
//-----------------------------------------------------------------------------------------------------------------------
deque<UnicodeString> Trie::String2Deque(const UnicodeString & str)
{
 deque<UnicodeString> cont;
 Locale bilocale("ru_RU");
 UErrorCode status = U_ZERO_ERROR;
 UnicodeString word;
 int32_t start =0, end =0;

 unique_ptr<BreakIterator> wordIterator (BreakIterator::createWordInstance(bilocale, status));
 if(U_FAILURE(status))
    throw runtime_error(string(__FUNCTION__)+": Can't create word iterator...");

 wordIterator->setText(str);

    for(end = wordIterator->next();
        end != BreakIterator::DONE;
        start = end, end = wordIterator->next())
    {
       word = str.tempSubString(start,end-start);
       if (word==" "){continue;}

       cont.push_back(word);
    }

 return cont;
}
//-----------------------------------------------------------------------------------------------------------------------
bool Trie::CompleteText(const UnicodeString & startText,int iLength,enum Mode mode)
{
    UnicodeString word;
    deque<UnicodeString> vText;
    try
    {
        vText = String2Deque(startText);
        if(vText.size()==0)throw runtime_error("Start text is empty..."); //smth goes wrong

        cout<<startText << " ";
        while(iLength--)
        {

            if (mode == Mode::MostFreq)
                word = GetMostFrequentChildNode(vText);
            else if (mode == Mode::ProbDistrib)
                word = GetRandomChildNode(vText);

            if(word=="Node is not found") throw runtime_error("Processing stopped, n-gram is not found...");
            if(word=="Node child not found")throw runtime_error("Processing stopped, no child nodes for n-gram found...");
            cout<< word << " ";
            vText.push_back(word);
            vText.pop_front();
        }
     cout<<endl;
     vText.clear();
     return true;
    }
    catch (exception & e)
    {
        vText.clear();
        cout<<endl<<__FUNCTION__<<": "<<e.what()<<endl;
        return false;
    }
}
//-----------------------------------------------------------------------------------------------------------------------
bool Trie::BuildTrieFromFile(const string & filename)
{
   int const windowWidth = chainRank +2;
   struct stat st;
   int32_t length = 0,start=0,end=0, ruleStatus=0;
   int wordsCount = 0;
   UBreakIterator * bi = nullptr; // Conflicts with unique_ptr
   Locale bilocale("ru_RU");
   UErrorCode status = U_ZERO_ERROR;
   int totalWords=0;
   ifstream fin;
   deque<UnicodeString> nGram; // Fast insertion and removal from both sides O(1)
   UnicodeString usContent,word;
   unique_ptr<char[]> rawContent;

   if (windowWidth<=1)
    {cout<< "windowWidth is less than 2. Supported values are: from 2 to words count in the corpus/string/file..."<< endl;return false;}



  try{
     fin.exceptions(std::ios_base::badbit|std::ios_base::failbit);
     fin.open(filename.c_str(),std::ios_base::in|std::ios_base::binary);
     if(!fin.is_open()){throw runtime_error("Error: can't open file: "+filename);}

     //Calculating the file size:
     stat(filename.c_str(), &st);
     length = st.st_size;

     //Read entire file
     rawContent.reset(new char[length+1]);
     fin.read(rawContent.get(),length);
     rawContent[length]='\0';
     if (!fin || length!=fin.gcount())throw runtime_error("Error reading "+filename);
     fin.close();

     //Build UTF8 content
     usContent = UnicodeString::fromUTF8(rawContent.get());
     //Freeing memory
     rawContent.reset();

     //Create break iterator
     bi = ubrk_open(UBRK_WORD, "ru_RU", usContent.getBuffer(), usContent.length(), &status);
     if(U_FAILURE(status))
        throw std::runtime_error("Can't create break iterator object...");


     for(end = ubrk_next(bi);end != UBRK_DONE;start = end, end = ubrk_next(bi))
     {
        ruleStatus= ubrk_getRuleStatus(bi);
        if (ruleStatus == UBRK_WORD_NONE || ruleStatus == UBRK_WORD_NUMBER)// Count only words (except numbers, spaces & punctuation)
            continue;

            //Extract iterated word
            word = usContent.tempSubString(start,end-start).toLower();

            if(wordsCount == windowWidth)
            {
                //Forward nGram to trie
                if (!InsertNGram(nGram))throw std::runtime_error("Can't insert n-gram into tree...");

                //delete first element
                nGram.pop_front();

                // add current element
                nGram.push_back(word);
            }
            else{
                nGram.push_back(word);
                wordsCount+=1;
            }
        totalWords++;
    }

     //Forward last nGram to trie
     if (!InsertNGram(nGram))throw std::runtime_error("Can't insert n-gram into tree...");

    nGram.clear();
    ubrk_close(bi);

    cout<<__FUNCTION__<<": Totalwords:"<<totalWords<<endl;
    return true;
  }
  catch(const exception &e)
  {
   cout<<__FUNCTION__<<": "<<e.what()<<endl;
   if(nGram.size()!=0)nGram.clear();
   if(bi)ubrk_close(bi);
   if(fin.is_open())fin.close();
   return false;
  }
}
//-----------------------------------------------------------------------------------------------------------------------
bool Trie::BuildTrieFromFolder(const string & folder)
{
    DIR *pDir = NULL;
    struct dirent *entry;
    struct stat statbuf;
    char currentDir[256];

    try
    {
        pDir = opendir(folder.c_str());
        if(!pDir){throw runtime_error("Can't open directory with corpus files...");}

        if(!getcwd(currentDir,256)){strcpy(currentDir,folder.c_str());}

        if(chdir(folder.c_str())==-1){throw runtime_error("Can't set current directory to "+ folder);}
        while((entry = readdir(pDir)) != NULL)
            {
             lstat(entry->d_name,&statbuf);

             if(S_ISDIR(statbuf.st_mode)){continue;}

             cout<<"=> Loading file: "<<folder<<"/"<<entry->d_name<<" into trie tree..."<<endl;

             if(!BuildTrieFromFile(string(entry->d_name))){throw runtime_error("Can't build trie from file "+string(entry->d_name));}
            }
        if(chdir(currentDir)==-1){throw runtime_error("Can't set current directory to "+ string(currentDir));}
        if(pDir){closedir(pDir);}
        return true;
    }
    catch(const exception &e)
    {
      cout<<__FUNCTION__<<": "<<e.what()<<endl;
      if(pDir){closedir(pDir);}
      return false;
    }
}
//-----------------------------------------------------------------------------------------------------------------------
void Trie::WriteNodeToFile(ofstream & fout,const Node & node)
{
    size_t childNodesCount = node.second->childNodes.size();
    unsigned short uiCount = node.second->count;
    int32_t keyLength = node.first.length();

    fout.write((char*)&uiCount,sizeof(uiCount)); //Node count
    fout.write((char*)&childNodesCount,sizeof(childNodesCount));//child nodes count

    //std::cout<<"writing node: "<<key<<std::endl;
    fout.write((char*)&keyLength,sizeof(keyLength)); //key length
    fout.write((char*)node.first.getBuffer(),sizeof(UChar)*keyLength); //key
    //std::cout<<"writing node: "<<key2<<std::endl;

    for (auto keyValuePair : node.second->childNodes)
      WriteNodeToFile(fout,keyValuePair);

    return;
}
//-----------------------------------------------------------------------------------------------------------------------
bool Trie::SerializeTree(const string & filename)
{
  ofstream fout;
  size_t childNodesCount = rootNode.second->childNodes.size();
  cout<<"=> Start dumping Markov chain into file: "<<filename<<endl;

  try
  {
    fout.exceptions(std::ios_base::badbit|std::ios_base::failbit); //throw exceptions in case of any write error
    fout.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc|std::ios_base::binary);

    if (!fout.is_open()) {throw std::runtime_error("Can't create file for serializing trie tree...");}

    //write general info
    fout.write((char*)&chainRank,sizeof(chainRank)); //Rank
    fout.write((char*)&childNodesCount,sizeof(childNodesCount)); //Root node child count

    for (auto keyValuePair : rootNode.second->childNodes)
        WriteNodeToFile(fout,keyValuePair);

    fout.close();
    return true;
  }
  catch(const exception &e)
  {
   cout<<__FUNCTION__<<": "<<e.what()<<endl;
   if(fout.is_open()){fout.close();}
   return false;
  }
}
//-----------------------------------------------------------------------------------------------------------------------
void Trie::ReadNodeFromFile(ifstream & fin, Node & node)
{
    size_t childNodesCount = 0;
    unsigned short uiCount = 0;
    int32_t keyLength = 0;
    Container::iterator itNode;

    fin.read((char*)&uiCount,sizeof(unsigned short));
    fin.read((char*)&childNodesCount,sizeof(size_t));
    fin.read((char*)&keyLength,sizeof(int32_t));

    unique_ptr<UChar[]> buffer(new UChar[keyLength]);

    fin.read((char *)buffer.get(),sizeof(UChar)*keyLength);

    UnicodeString key(buffer.get(),keyLength);

    //std::cout<<"reading node: "<<key<<std::endl;

    auto res = node.second->childNodes.insert(Node(key,shared_ptr<NodeElement>(new NodeElement(uiCount))));
    if (!res.second) throw runtime_error("Can't insert node into tree...");


    while(childNodesCount--)
        ReadNodeFromFile(fin,*res.first);

    return;
}
//-----------------------------------------------------------------------------------------------------------------------
bool Trie::DeserializeTree(const string & filename)
{
  ifstream fin;
  size_t childNodesCount;

  cout<<"=> Start loading Markov chain from file: "<<filename<<endl;

  try
  {
      fin.exceptions(std::ios_base::badbit|std::ios_base::failbit|std::ios_base::eofbit); //throw exceptions in case of any read error
                                                                                          //(we need to read all the data and the data should
                                                                                          // fit exactly into data structure, so no eof should encountered
                                                                                          // while reading.
      fin.open(filename.c_str(),std::ios_base::in|std::ios_base::binary);
      if (!fin.is_open()) {throw std::runtime_error("Can't open file "+filename);}

      //Clear current tree
      for (auto & keyValuePair : rootNode.second->childNodes)
            DeleteNode(keyValuePair);

      if(rootNode.second->childNodes.size()!=0)
            rootNode.second->childNodes.clear();
      //
      rootNode.second->count = 0;

      //read general info
      fin.read((char*)&chainRank,sizeof(chainRank)); //Rank
      fin.read((char*)&childNodesCount,sizeof(childNodesCount)); //Root node child count

      while(childNodesCount--)
        ReadNodeFromFile(fin,rootNode);

      fin.close();
      return true;
  }
  catch(const std::exception &e)
  {
   cout<<__FUNCTION__<<": "<<e.what()<<endl;
   if(fin.is_open())fin.close();
   return false;
  }

}
//-----------------------------------------------------------------------------------------------------------------------
Trie::iterator Trie::begin()
{
    return iterator(&rootNode,lastNode);
}

Trie::iterator Trie::end()
{
    return iterator(lastNode +1,lastNode);
}
//-----------------------------------------------------------------------------------------------------------------------
//#
//# ITERATOR
//#
//-----------------------------------------------------------------------------------------------------------------------
template <typename ValueType>
bool Trie::TrieIterator<ValueType>::operator!=(const TrieIterator & it) const
{
 if (p != &*it)
    return true;
 else
    return false;
}
//-----------------------------------------------------------------------------------------------------------------------
template <typename ValueType>
bool Trie::TrieIterator<ValueType>::operator==(const TrieIterator & it) const
{
 if (p == &*it)
    return true;
 else
    return false;
}
//-----------------------------------------------------------------------------------------------------------------------
template <typename ValueType>
ValueType & Trie::TrieIterator<ValueType>::operator*() const
{
 return *p;
}
//-----------------------------------------------------------------------------------------------------------------------
template <typename ValueType>
Trie::TrieIterator<ValueType> & Trie::TrieIterator<ValueType>::operator++()
{
  if (dir == direction::down) // drill down
      {
        futureParent = nullptr;

        chNodeIteratorPos = curParent->second->childNodes.begin();
           auto size =  curParent->second->childNodes.size();
           if (size != 0)
           {
             p = &*chNodeIteratorPos;
             childProcessed = size-1; // one node is processed already
             dir = direction::right;

             //Set the future parent
             if (p->second->childNodes.size()!=0)
                futureParent = p;
           }
           else //up and right, if nowhere to go, then we reach the end
           {
            if (isRoot(curParent))
            {
              p = pLn+1;
              return *this;
            }

            auto pNodeIteratorPos = curParent->second->parent->second->childNodes.find(curParent->first);
            ++pNodeIteratorPos;
              if (pNodeIteratorPos == curParent->second->parent->second->childNodes.end())
                {
                   dir = direction::right;
                   curParent = curParent->second->parent;
                   return operator++();
                }
              curParent = &*pNodeIteratorPos;
              return operator++();
           }
      }
      else //direction::right
       {
         if (childProcessed == 0)
         {
            dir = direction::down; // try to go down

            //Set the next parent
            if (futureParent)
             curParent = futureParent; //drill down to one of the child node of the curParent
            //Can't drill down, no candidates among nodes, just processed...
            else //up and right, if nowhere to go, then we reach the end
            {

              if (isRoot(curParent))
              {
                p = pLn+1;
                return *this;
              }

              auto pNodeIteratorPos = curParent->second->parent->second->childNodes.find(curParent->first);
              ++pNodeIteratorPos;
              if (pNodeIteratorPos == curParent->second->parent->second->childNodes.end())
                {
                   dir = direction::right;
                   curParent =  curParent->second->parent;
                   return operator++();
                }
              curParent = &*pNodeIteratorPos;
              return operator++();

            }
            return operator++();
         }

         p =&*(++chNodeIteratorPos);
         if ((!futureParent) & (p->second->childNodes.size()!=0))
            futureParent = p;

         childProcessed--;
       }

 return *this;
}
//-----------------------------------------------------------------------------------------------------------------------
template <typename ValueType>
bool Trie::TrieIterator<ValueType>::isRoot(const ValueType* p)
{
 if (p->second->parent)
    return false;
 else
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------

//Force compiler to generate instances:
template bool Trie::TrieIterator<Trie::Node>::operator!=(const TrieIterator & it) const;
template bool Trie::TrieIterator<Trie::Node>::operator==(const TrieIterator & it) const;
template Trie::Node & Trie::TrieIterator<Trie::Node>::operator*() const;
template Trie::TrieIterator<Trie::Node>& Trie::TrieIterator<Trie::Node>::operator++();

