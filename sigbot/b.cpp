string stringy(int timestamp){
    const time_t rawtime = (const time_t)timestamp;
    struct tm * dt;
    char timestr[30];
    char buffer [30];
    dt = localtime(&rawtime);
    // use any strftime format spec here
    strftime(timestr, sizeof(timestr), "%Y-%m-%d", dt);
    sprintf(buffer,"%s", timestr);
    std::string stdBuffer(buffer);
    return stdBuffer;
}

string d_str(float d){
    stringstream stream;
    stream << fixed << setprecision(2) << d;
    string str = stream.str();
    
    cout << str << "     ";
    size_t found = str.find(".");
    if(found != string::npos){//find uses 0 indexing, so size()-1
        if( (str.size()-1) - found == 1 )
            str+= "0";
    }else str+=".00";
    
    if( (str.size()-1)-found > 2 ){
        str.erase( found+2, str.size()-1-(found+2) );
    }
    cout << "d_str " << d << " -> " << str << endl;
    return str;
}

struct Data{
public:
    int ts;
    string inst;
    string side;
    float pr;
    int size;
    Data(int p1, string p2, string p3, float p4, int p5){
        ts = p1;
        inst = p2;
        side = p3;
        pr = p4;
        size = p5;
    }
    
    bool operator < (const Data& str) const{ return (ts < str.ts); }
};

struct Stock{
public:
    string time, name;
    float open, high, low, close;
    
    Stock(string ts, string n, float openval){
        time = ts;
        name = n;
        open = high = low = close = openval;
    }
};

std::vector<std::vector<std::string>> dailyOHLC(std::vector<int> timestamp, std::vector<std::string> instrument, std::vector<std::string> side, std::vector<double> price, std::vector<int> size) {
    vector<Data> d; vector<Stock> s; vector<vector<string>> r;
    
    //first key is the timestamp, the second key is the company name
    unordered_map<string, unordered_map<string, Stock>> mymap;
    unordered_map<string, unordered_map<string, Stock>>::iterator key1;
    unordered_map<string, Stock>::iterator key2;
    
    
    for(int i = 0; i < timestamp.size(); i++){
        d.push_back( Data(
                          timestamp[i],
                          instrument[i],
                          side[i],
                          price[i],
                          size[i]
                          )
                    );
    }
    sort( d.begin(),d.end() );
    
    string tkey, nkey;
    for(int i = 0; i < timestamp.size(); i++){
        tkey = stringy(timestamp[i]);
        nkey = instrument[i];
        cout << stringy(timestamp[i]) << " " << nkey<<endl;
        key1 = mymap.find(tkey);
        if( key1 == mymap.end() ){//current time exists as a key
            unordered_map<string, Stock> tempmap;
            mymap.insert( make_pair(tkey, tempmap) );
            key1 = mymap.find(tkey);
        }
        
        key2 = key1->second.find(nkey);
        if( key2 != key1->second.end() ){//the company name (nkey) exists as well for tkey
            
            Stock tstock = key2->second;
            if( tstock.high < price[i] ) tstock.high = price[i];
            if( tstock.low > price[i] ) tstock.low = price[i];
            tstock.close = price[i];
            key2->second = tstock;
        }else{
            key1->second.insert(make_pair( nkey, Stock(tkey, nkey, price[i]) ) );
        }
    }
    
    for(auto& a1: mymap ){
        for(auto& a2: a1.second){
            vector<string> vs;
            vs.push_back( a1.first);
            vs.push_back(a2.first);
            vs.push_back( d_str(a2.second.open) );
            vs.push_back( d_str(a2.second.high) );
            vs.push_back( d_str(a2.second.low));
            vs.push_back( d_str(a2.second.close) );
            r.push_back(vs);
        }
    }
    
    std::sort(r.begin(), r.end(), [](const std::vector< string >& a, const std::vector< string >& b){
        if( a[0] == b[0] ) return a[1] < b[1];
        else return a[0] < b[0];
    });
    
    return r;
}
