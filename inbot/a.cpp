instabot ac3


struct vari{
    int index;
    int count;
    bool used;
    vari(int i){ this->index = i; this->count = 0; this->used = false;}
    bool operator < (const vari& str) const{ return (count < str.count); }
    bool use(){ return this->used; }
    void on(){ this->used = true;}
    void off(){ this->used = false;}
    int i(){return this->index;}
};

struct crawler{
    vector<bool> bused;
    vector<vector<int>> options;
    
    vector<int> path;
    vector<pair<int,int>> pathcoords;
    vector<vector<int>> used;
    int count;//essentially the row index
    
    crawler(){}
    void push( int x, int y, int p, int s, int o){
        // this->pathcoords.push_back(make_pair(x,y ));
        this->path.push_back(p);
        this->bused = vector<bool>(s,false);
        this->bused[p] = true;
        for(int i = 0; i < o; i++){
            this->pathcoords.push_back(make_pair(i, 0));
        }
    }
    
    vector<int> isMove(const vector<int>& v){
        vector<int> r;
        for(int i = 0; i < v.size(); i++){
            if( bused[v[i]] == false ){
                r.push_back( v[i] );
            }
        }
        return r;
    }
    
    void incswap(int x, int n){
        this->bused[this->path[this->path.size()-1]] = false;
        this->path.pop_back();
        this->bused[n] = true;
        this->path.push_back(n);
    }
    void backup(){
        this->bused[this->path[this->path.size()-1]] = false;
        this->path.pop_back();
        this->options.pop_back();
        //this->pathcoords.pop_back();
    }
    int basex(){
        return this->pathcoords[0].first;
    }
    int basey(){
        return this->pathcoords[0].second;
    }
};

int toMin(string s){
    auto pos = s.find(':');
    //cout << "toMin " << s.substr(0,pos) << "  " << s.substr(pos+1) << endl;
    return stoi(s.substr(0,pos))*60 + stoi(s.substr(pos+1));
}

unordered_map<int,vari> mymap;
unordered_map<int,vari>::iterator it1;
unordered_map<int,vari>::iterator it2;


bool busyHolidays(std::vector<std::vector<std::string>> shoppers, std::vector<std::vector<std::string>> orders, std::vector<int> leadTime) {
    bool pr = false;
    vector<vector<int>> shop;
    vector<vector<int>> ord;
    
    vector<vector<int>> fit;
    
    vector<vari> arr;
    for(int i = 0; i < shoppers.size(); i++){
        vector<int> t;
        t.push_back( toMin(shoppers[i][0]) );
        t.push_back( toMin(shoppers[i][1]) );
        shop.push_back( t );
        arr.push_back( vari(i) );
        if(i < orders.size()){
            vector<int> o;
            o.push_back( toMin(orders[i][0]) );
            o.push_back( toMin(orders[i][1]) );
            o.push_back( leadTime[i] );
            ord.push_back( o );
        }
    }
    sort(shop.begin(), shop.end(), [](const std::vector< int >& a, const std::vector< int >& b){ return a[0] < b[0]; } );
    sort(ord.begin(), ord.end(), [](const std::vector< int >& a, const std::vector< int >& b){ return a[0] < b[0]; } );
    
    //int count = 0;
    //int i=  0;
    int s1,s2,o1,o2,t;
    for(int count = 0; count < ord.size(); count++){
        cout << "order: " << ord[count][0] << " - " << ord[count][1] << " | " << ord[count][2] << endl;
        o1 = ord[count][0];
        o2 = ord[count][1];
        t = ord[count][2];
        vector<int> te;
        fit.push_back(te);
        for(int i = 0; i < shop.size(); i++){
            cout << "shopper: " << shop[i][0] << " - " << shop[i][1] << endl;
            s1 = shop[i][0];
            s2 = shop[i][1];
            if( (s1 <= o1) && (o1 <= o2) && (o2 <= s2) && o1+t <= o2 ){//case 1
                fit[count].push_back(i);
                arr[i].count++;
            }else if( (o1<=s1) && (s1 <=s2) && (s2 <=o2) && s1+t<=s2 ){ //case 2
                fit[count].push_back(i);
                arr[i].count++;
            }else if( (s1<=o1)&&(o1<=s2)&&(s2<=o2)&&o1+t<=s2 ){
                fit[count].push_back(i);
                arr[i].count++;
            }else if( (o1<=s1)&&(s1<=o2)&&(o2<=s2)&&s1+t<=o2 ){
                fit[count].push_back(i);
                arr[i].count++;
            }
        }
    }
    sort(arr.begin(), arr.end());
    
    bool c1 = false;
    int eq = ord.size();
    for(int i = 0; i < fit.size(); i++){
        if(fit[i].size() < eq)
            c1 = true;
    }
    //special case: if all arcs have sizes greater than the size of orders, then it is possible
    if(c1 == false) return true;
    
    
    for(int i = 0; i < shop.size(); i++){
        cout << arr[i].i() << " count: " << arr[i].count << endl;
        mymap.insert( make_pair(arr[i].i(), arr[i]) );
    }
    
    sort( fit.begin(), fit.end(),[](const std::vector< int >& a, const std::vector< int >& b){ return a.size() < b.size(); } );
    
    for(int i = 0; i < fit.size(); i++){
        cout << "order index " << i << ": ";
        if(fit[i].size() == 0)
            return false;
        if(pr){
            for(int j = 0; j < fit[i].size(); j++){
                cout << fit[i][j]<<" ";
            }
            cout << endl;
        }
    }
    
    int set = 0;
    int ind = 0;
    int available = arr.size();//HOLDS THE NUMBER OF AVAILABLE UNASSIGNED VARIABLES
    vector<int>::iterator p;
    int r = 1;//range
    bool case2 = false;
    //else, got to do that damned ac3 algorithm
    while( fit.size() != 0 ){//if a vector is ever to become empty, return false
        if(fit[ind%fit.size()].size() == 0){
            cout << "false case " << ind<<endl;
            return false;
        }
        cout << "set/ind/r " << set << "/"<<ind<<"/" <<r << endl;
        if(fit[ind].size() == 1){
            set++;
            int siz;
            for(int i = ind+1; i < ord.size(); i++){
                siz = i%fit.size();
                p = find( fit[siz].begin(), fit[siz].end(), fit[ind][0] );
                if(p != fit[siz].end() ){
                    cout << "p-> " << *p << endl;
                    fit[siz].erase(p);
                }
            }
            fit.erase(fit.begin()+ind);
            sort( fit.begin(), fit.end(),[](const std::vector< int >& a, const std::vector< int >& b){ return a.size() < b.size(); } );
        }else{ cout << "else " << endl;
            //sort( fit.begin(), fit.end(),[](const std::vector< int >& a,
            // const std::vector< int >& b){ return a.size() < b.size(); } );
            //what is the scenario where there is nothing left to do but increment?
            // I BELIEVE that this else case where backtracking is necessary
            for(int d = 0; d < fit.size();d++)
                sort( fit[d].begin(), fit[d].end(), [&](const int x, const int y){ return mymap.find(x)->second.count > mymap.find(y)->second.count; });
            
            set = 0;
            crawler c;
            c.push(0,0,fit[0][0], shop.size(), ord.size());
            int x=0;
            int y = 0;
            // crawler can either check domain values, backtrack, or forward track
            while( y < fit.size() ){
                //if a vector of options is appended for the last order, there is a
                //shopper for each order
                if(y < 0 || (y == 0 && x == fit[0].size()) ) return false;
                if( y == fit.size()-1) return true;
                
                vector<int> m = c.isMove(fit[y+1]);
                if( y < fit.size() && m.size() > 0 ){
                    //can move forward, append current path coordinates as well as the
                    //new row of options
                    c.pathcoords[y].second = x;
                    c.options.push_back(m);
                    y++;
                    if( y == fit.size()-1) return true;
                    x = 0;
                }else if(  y < fit.size() && x < c.options[y].size()-1 ){
                    // current option has no way of moving on, use the next possible
                    c.incswap(x, c.options[y][x+1]);
                    x++;
                }else if( x == c.options[y].size()-1 ){//all of the possible options are exhausted
                    y--;
                    c.backup();
                    x = c.pathcoords[y].second;
                    while(x == c.options[y].size()-1){
                        y--;
                        if(y < 0) return false;
                        c.backup();
                    }
                    c.incswap(x, c.options[y][x+1]);
                    x++;
                }
                if(y < 0 || (y == 0 && x == fit[0].size()) ) return false;
                if( y == fit.size()-1) return true;
            }
        }
        cout << "size " << fit.size() << endl;
    }
    
    return true;
}