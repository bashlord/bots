#define OFFSET 97
#define ASCII_VALUE 97
#define OFFSET_ALPHA 65
#define MAXSIZE 26

class Trie;

class TrieNode{
public:
    TrieNode (char chr,bool used,bool end):chr(chr){
        END = end;
        USED = used;
        point = 0;
    }
    char chr; // character to be inserted
    Trie* point;  // Pointer to Trie
    bool END; // Boolean for for an end of character of word
    bool USED;    // Boolean flag if the word has been checked
};

class Trie{
public:
    TrieNode* nodes[(MAXSIZE*2)+42];
    vector<string> words;
    string type;
    string id;
    string score;
    long int timer;
    
    /**
     * Constructor that creates array of TrieNodes
     */
    Trie(){
        for(int i=0;i<MAXSIZE;i++){
            nodes[i] = new TrieNode((char)i+OFFSET,false,false);
        }
        for(int i=0;i<MAXSIZE;i++){
            nodes[i+26] = new TrieNode((char)i+OFFSET_ALPHA,false,false);
        }
        nodes[MAXSIZE*2] = new TrieNode((char)45,false,false);
        nodes[MAXSIZE*2+1] = new TrieNode((char)39,false,false);
        nodes[MAXSIZE*2+2] = new TrieNode((char)63,false,false);
        nodes[MAXSIZE*2+3] = new TrieNode((char)46,false,false);
        nodes[MAXSIZE*2+4] = new TrieNode((char)44,false,false);
        nodes[MAXSIZE*2+5] = new TrieNode((char)59,false,false);
        nodes[MAXSIZE*2+6] = new TrieNode((char)58,false,false);
        nodes[MAXSIZE*2+7] = new TrieNode((char)33,false,false);
        
        int j = 0;
        for(int i = MAXSIZE*2+8; i < MAXSIZE*2+13; i++){
            nodes[i] = new TrieNode((char)34+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+13; i < MAXSIZE*2+17; i++){
            nodes[i] = new TrieNode((char)40+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+17; i < MAXSIZE*2+28; i++){
            nodes[i] = new TrieNode((char)47+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+28; i < MAXSIZE*2+31; i++){
            nodes[i] = new TrieNode((char)60+j,false,false);
            j++;
        }
        //63 is skipped
        nodes[31] = new TrieNode((char)64,false,false);
        j = 0;
        for(int i = MAXSIZE*2+32; i < MAXSIZE*2+38; i++){
            nodes[i] = new TrieNode((char)91+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+38; i < MAXSIZE*2+42; i++){
            nodes[i] = new TrieNode((char)123+j,false,false);
            j++;
        }
        
        
    }
    
    Trie(string type, string id, string score, int t){
        
        this->timer = t;
        this->type = type;
        this->id = id;
        this->score = score;
        for(int i=0;i<MAXSIZE;i++){
            nodes[i] = new TrieNode((char)i+OFFSET,false,false);
        }
        for(int i=0;i<MAXSIZE;i++){
            nodes[i+26] = new TrieNode((char)i+OFFSET_ALPHA,false,false);
        }
        nodes[MAXSIZE*2] = new TrieNode((char)45,false,false);
        nodes[MAXSIZE*2+1] = new TrieNode((char)39,false,false);
        nodes[MAXSIZE*2+2] = new TrieNode((char)63,false,false);
        nodes[MAXSIZE*2+3] = new TrieNode((char)46,false,false);
        nodes[MAXSIZE*2+4] = new TrieNode((char)44,false,false);
        nodes[MAXSIZE*2+5] = new TrieNode((char)59,false,false);
        nodes[MAXSIZE*2+6] = new TrieNode((char)58,false,false);
        nodes[MAXSIZE*2+7] = new TrieNode((char)33,false,false);
        int j = 0;
        for(int i = MAXSIZE*2+8; i < MAXSIZE*2+13; i++){
            nodes[i] = new TrieNode((char)34+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+13; i < MAXSIZE*2+17; i++){
            nodes[i] = new TrieNode((char)40+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+17; i < MAXSIZE*2+28; i++){
            nodes[i] = new TrieNode((char)47+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+28; i < MAXSIZE*2+31; i++){
            nodes[i] = new TrieNode((char)60+j,false,false);
            j++;
        }
        //63 is skipped
        nodes[MAXSIZE*2+31] = new TrieNode((char)64,false,false);
        j = 0;
        for(int i = MAXSIZE*2+32; i < MAXSIZE*2+38; i++){
            nodes[i] = new TrieNode((char)91+j,false,false);
            j++;
        }
        j = 0;
        for(int i = MAXSIZE*2+38; i < MAXSIZE*2+42; i++){
            nodes[i] = new TrieNode((char)123+j,false,false);
            j++;
        }
        
    }
    
    int getSize(){return sizeof(nodes);}
    void addWord(string str){ words.push_back(str);}
    
    void insert(Trie* trie,string word){
        if(word.length()==0)
            return;
        int i =0;
        while(word[0] != trie->nodes[i]->chr){
            i++;
        }
        
        if(trie->nodes[i]->USED==false){//turns on the used flag
            trie->nodes[i]->USED=true;
            if(word.length()==1)
                trie->nodes[i]->END=true;//turns on the end word flag
            trie->nodes[i]->point = new Trie();
        }
        insert(trie->nodes[i]->point,word.substr(1,word.length() ) ) ; //recurses
    }
    
    bool search(Trie* trie,string word){
        string str = word;
        bool done = false;
        
        TrieNode** nodes1 = trie->nodes;
        int off = 0;
        if((int)word[0] < 97)
            off = 29;
        else
            off = ASCII_VALUE;
        int index = word[0] - off;
        
        if((int) word[0] == 45){
            index = 52;
        }else if((int) word[0] == 39){
            index = 53;
        }else if((int) word[0] == 63){
            index = 54;
        }else if((int) word[0] == 46){
            index = 55;
        }else if((int) word[0] == 44){
            index = 56;
        }else if((int) word[0] == 59){
            index = 57;
        }else if((int) word[0] == 58){
            index = 58;
        }else if((int) word[0] == 33){
            index = 59;
        }else if((int) word[0] == 34){
            index = 60;
        }else if((int) word[0] == 35){
            index = 61;
        }else if((int) word[0] == 36){
            index = 62;
        }else if((int) word[0] == 37){
            index = 63;
        }else if((int) word[0] == 38){
            index = 64;
        }else if((int) word[0] == 40){
            index = 65;
        }else if((int) word[0] == 41){
            index = 66;
        }else if((int) word[0] == 42){
            index = 67;
        }else if((int) word[0] == 43){
            index = 68;
        }else if((int) word[0] == 47){
            index = 69;
        }else if((int) word[0] == 48){
            index = 70;
        }else if((int) word[0] == 49){
            index = 71;
        }else if((int) word[0] == 50){
            index = 72;
        }else if((int) word[0] == 51){
            index = 73;
        }else if((int) word[0] == 52){
            index = 74;
        }else if((int) word[0] == 53){
            index = 75;
        }else if((int) word[0] == 54){
            index = 76;
        }else if((int) word[0] == 55){
            index = 77;
        }else if((int) word[0] == 56){
            index = 78;
        }else if((int) word[0] == 57){
            index = 79;
        }else if((int) word[0] == 60){
            index = 80;
        }else if((int) word[0] == 61){
            index = 81;
        }else if((int) word[0] == 61){
            index = 82;
        }else if((int) word[0] == 64){
            index = 83;
        }else if((int) word[0] == 91){
            index = 84;
        }else if((int) word[0] == 92){
            index = 85;
        }else if((int) word[0] == 93){
            index = 86;
        }else if((int) word[0] == 94){
            index = 87;
        }else if((int) word[0] == 95){
            index = 88;
        }else if((int) word[0] == 96){
            index = 89;
        }else if((int) word[0] == 123){
            index = 90;
        }else if((int) word[0] == 124){
            index = 91;
        }else if((int) word[0] == 125){
            index = 92;
        }else if((int) word[0] == 126){
            index = 93;
        }
        while( !done ){
            if( nodes1[index]->USED == true ){
                if(str.length() == 1 && nodes1[index]->END == true){
                    return true;
                }
                else if( str.length() == 1 ){
                    return true;
                }
                nodes1 = nodes1[index]->point->nodes;
                str = str.substr(1,str.length());
                if((int)str[0] < 97)
                    off = 29;
                else
                    off = ASCII_VALUE;
                
                if((int)str[0] == 45){
                    index = 52;
                }else if((int)str[0] == 39){
                    index = 53;
                }else if((int)str[0] == 63){
                    index = 54;
                }else if((int)str[0] == 46){
                    index = 55;
                }else if((int)str[0] == 44){
                    index = 56;
                }else if((int)str[0] == 59){
                    index = 57;
                }else if((int)str[0] == 58){
                    index = 58;
                }else if((int)str[0] == 33){
                    index = 59;
                }else if((int)str[0] == 34){
                    index = 60;
                }else if((int)str[0] == 35){
                    index = 61;
                }else if((int)str[0] == 36){
                    index = 62;
                }else if((int)str[0] == 37){
                    index = 63;
                }else if((int)str[0] == 38){
                    index = 64;
                }else if((int)str[0] == 40){
                    index = 65;
                }else if((int)str[0] == 41){
                    index = 66;
                }else if((int)str[0] == 42){
                    index = 67;
                }else if((int)str[0] == 43){
                    index = 68;
                }else if((int)str[0] == 47){
                    index = 69;
                }else if((int)str[0] == 48){
                    index = 70;
                }else if((int)str[0] == 49){
                    index = 71;
                }else if((int)str[0] == 50){
                    index = 72;
                }else if((int)str[0] == 51){
                    index = 73;
                }else if((int)str[0] == 52){
                    index = 74;
                }else if((int)str[0] == 53){
                    index = 75;
                }else if((int)str[0] == 54){
                    index = 76;
                }else if((int)str[0] == 55){
                    index = 77;
                }else if((int)str[0] == 56){
                    index = 78;
                }else if((int)str[0] == 57){
                    index = 79;
                }else if((int)str[0] == 60){
                    index = 80;
                }else if((int)str[0] == 61){
                    index = 81;
                }else if((int)str[0] == 62){
                    index = 82;
                }else if((int)str[0] == 64){
                    index = 83;
                }else if((int)str[0] == 91){
                    index = 84;
                }else if((int)str[0] == 92){
                    index = 85;
                }else if((int)str[0] == 93){
                    index = 86;
                }else if((int)str[0] == 94){
                    index = 87;
                }else if((int)str[0] == 95){
                    index = 88;
                }else if((int)str[0] == 96){
                    index = 89;
                }else if((int)str[0] == 123){
                    index = 90;
                }else if((int)str[0] == 124){
                    index = 91;
                }else if((int)str[0] == 125){
                    index = 92;
                }else if((int)str[0] == 126){
                    index = 93;
                }else
                    index = str[0] - off;
            }
            else{
                //cout << "RETURNING FALSE " << nodes1[index]->chr << " " << index << endl;
                return false;
            }
        }
    }I
    bool query(vector<string> q){
        //cout << "LOOKING" << endl;
        for(int i = 0; i < q.size(); i++){
            //cout << q[i] << endl;
            if( this->search(this,q[i]) == false ){
                //cout << "returned false" << endl;
                return false;
            }
        }
        //cout << "returned true" << endl;
        return true;
    }
    
    void initLex(){
        for(std::vector<string>::iterator it=words.begin();it!=words.end();++it)
            insert(this,*it);
    }
    
};

class Utils{
public:
    
    void insertSet(const string word){
        lexSet.insert(word);//inserted word into the lex
    }
    
    void insert(Trie* trie,string word){
        if(word.length()==0)
            return;
        int i =0;
        while(word[0] != trie->nodes[i]->chr){
            i++;
        }
        if(trie->nodes[i]->USED==false){//turns on the used flag
            trie->nodes[i]->USED=true;
            if(word.length()==1)
                trie->nodes[i]->END=true;//turns on the end word flag
            trie->nodes[i]->point = new Trie();
        }
        insert(trie->nodes[i]->point,word.substr(1,word.length() ) ) ; //recurses
    }
    
    bool searchsearch(Trie* trie,string word){
        string str = word;
        bool done = false;
        TrieNode** nodes1 = trie->nodes;
        int index = word[0] - ASCII_VALUE;
        while( !done ){
            
            if( nodes1[index]->USED == true ){
                
                if(str.length() == 1 && nodes1[index]->END == true)
                    return true;
                
                else if( str.length() == 1 )
                    return true;
                
                nodes1 = nodes1[index]->point->nodes;
                str = str.substr(1,str.length());
                index = str[0] - ASCII_VALUE;
            }
            else
                return false;
        }
    }
    
    set<string> lexSet;
    int lexSetCount(){ return lexSet.size();}
};

Utils util;
unordered_map<string,Trie*> mymap;
unordered_map<string,Trie*>::const_iterator it;
std::vector<std::vector<std::string>> typeaheadSearch(std::vector<std::vector<std::string>> queries) {
    vector<vector<string>> ret;
    //cout << queries.size() << endl;
    for(int i = 0; i < queries.size(); i++){
        //cout << "---------- " << i << " - " <<queries.size() << "--------------------"<<endl;
        if(queries[i][0] == "ADD"){
            //cout << "add" << endl;
            Trie* t = new Trie(queries[i][1],queries[i][2],queries[i][3], i );
            //cout << "add 1" << endl;
            vector<std::string> result;
            istringstream iss(queries[i][4]);
            for(std::string s; iss >> s; ){
                
                transform(s.begin(), s.end(), s.begin(), ::tolower);
                //cout << s << endl;
                result.push_back(s);
                t->addWord(s);
            }
            //cout << "add 2" << endl;
            t->initLex();
            //cout << "add 3" << endl;
            mymap.insert( make_pair(queries[i][2], t));
        }else if(queries[i][0] == "DEL"){
            //cout << "del" << endl;
            mymap.erase(queries[i][1]);
        }else if(queries[i][0] == "QUERY"){
            //cout << "queries" << endl;
            vector<string> q;
            vector<pair<string,pair<string, long int>>> r;
            vector<string> fin;
            istringstream iss(queries[i][2]);
            for(std::string s; iss >> s; ){
                transform(s.begin(), s.end(), s.begin(), ::tolower);
                //cout << "que " << s << endl;
                q.push_back(s);
            }
            
            for ( auto it = mymap.begin(); it != mymap.end(); ++it ){
                if(it->second->query(q) ){
                    r.push_back(make_pair(it->second->id, make_pair(it->second->score, it->second->timer) ));
                }
            }
            //cout << "q1" << endl;
            sort(r.begin(), r.end(), [](auto &left, auto &right) {
                if( left.second.first != right.second.first)
                    return left.second.first > right.second.first;
                else
                    return left.second.second > right.second.second;
            });
            //cout << "q2" << endl;
            int limit =stoi(queries[i][1]);
            if(stoi(queries[i][1]) > r.size())
                limit = r.size();
            for(int a = 0; a < limit; a++){
                fin.push_back(r[a].first);
            }
            //cout << "q3" << endl;
            ret.push_back(fin);
        }
    }
    
    return ret;
}



std::string displayDiff(std::string oldVersion, std::string newVersion) {
    string r = "";
    int min = 0;
    if(oldVersion.size() > newVersion.size())
        min = newVersion.size();
    else
        min = oldVersion.size();
    
    int i = 0;
    int j = 0;
    while(i < min && j < min){
        cout << i << "-" <<oldVersion[i]<< " : " << j<<"-"<<newVersion[j] << endl;
        if( oldVersion[i] == newVersion[j] ){
            cout << "default adding " << oldVersion[i]<< " to " << r<<endl;
            r+= oldVersion[i];
            i++;
            j++;
            //continue;
        }else{
            int a = i;
            int b = j;
            bool flop = true;
            while(a < oldVersion.size() && b < newVersion.size()){
                if( oldVersion[i] == newVersion[a] && oldVersion[b] == newVersion[j] ){
                    //both edits so far have been done at the same time and are the
                    //same length
                    cout << "both" << endl;
                    string aa = "("+oldVersion.substr(i, a-i)+")";
                    string bb = "["+newVersion.substr(j, b-j) + "]";
                    if( aa.compare(bb)<0 ){
                        r+= aa;
                        r+= bb;
                        i = a;
                        j = b;
                        break;
                    }else{
                        r+= bb;
                        r+= aa;
                        i = a;
                        j = b;
                        break;
                    }
                }else if(newVersion[j] ==oldVersion[a] ){
                    
                    string bb = "("+oldVersion.substr(i, a-i)+")";
                    i = a;
                    cout << "OLDER VERSION EDIT adding " << bb<<" to " <<r <<endl;
                    r+= bb;
                    
                    break;
                }else if(oldVersion[i] == newVersion[b] ){//the new version was the edit
                    
                    string aa = "["+newVersion.substr(j, b-j)+"]";
                    cout << "new version editedadding " << aa<<" to " <<r <<endl;
                    j = b;
                    r+= aa;
                    break;
                }else if(newVersion[b] == oldVersion[a] && a-i == b-j){
                    cout << a << b<<endl;
                    cout << newVersion[a] << oldVersion[b]<<endl;
                    
                    string aa = "("+oldVersion.substr(i, a-i)+")";
                    string bb = "["+newVersion.substr(j, b-j) + "]";
                    cout << "same difference adding "<< aa << " " << bb << endl;
                    if( aa.compare(bb)<0 ){
                        r+= aa;
                        r+= bb;
                        i = a;
                        j = b;
                        cout << r << endl;
                        break;
                    }else{
                        r+= bb;
                        r+= aa;
                        i = a;
                        j = b;
                        cout << r << endl;
                        break;
                    }
                }
                a++;
                b++;
            }
            if(a == min && b == min){
                string aa = "("+oldVersion.substr(i, a-i)+")";
                string bb = "["+newVersion.substr(j, b-j) + "]";
                if( aa.compare(bb)<0 ){
                    r+= aa;
                    r+= bb;
                    i = a;
                    j = b;
                    break;
                }else{
                    r+= bb;
                    r+= aa;
                    i = a;
                    j = b;
                    break;
                }
            }
        }
    }
    int o = oldVersion.size();
    int n = newVersion.size();
    if(i < o && j < n){
        while(i < o|| j < n){
            cout << oldVersion[i]<< newVersion[j] << endl;
            if(oldVersion[i] == newVersion[j]){
                cout << 1 << endl;
                r+= oldVersion[i];
                i++;
                j++;
            }else if(o-i>n-j){
                cout << 2 << endl;
                r+= "(";
                r+=oldVersion[i];
                i++;
                while(oldVersion[i]!= newVersion[j]){
                    r+= oldVersion[i];
                    i++;
                }
                r+= ")";
            }else{
                cout << 3 << endl;
                r+= "[";
                r+= newVersion[j];
                j++;
                while(oldVersion[i]!= newVersion[j]){
                    r+= newVersion[j];
                    j++;
                }
                r+= "]";
            }
        }
    }
    else if(i < oldVersion.size()){
        r+="(";
        while(i<oldVersion.size()){
            r+=oldVersion[i];
            i++;
        }
        r+=")";
    } 
    else if(j < newVersion.size()){
        r+="[";
        while(j<newVersion.size()){
            r+=newVersion[j];
            j++;
        }
        r+="]";
    }
    return r;
}


