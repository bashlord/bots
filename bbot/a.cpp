bool isletter(char c){
    if( isalpha(c) || isdigit(c)|| c == '_' )
        return true;
    else
        return false;
}

bool plagiarismCheck(std::vector<std::string> code1, std::vector<std::string> code2) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    bool parse = false;
    string s1,s2;
    vector<string> test1,test2;
    test1 = code1;
    test2 = code2;
    
    int code1x = code1[code1.size()-1].size();
    int code2x = code2[code2.size()-1].size();
    int code1y = code1.size();
    int code2y = code2.size();
    unordered_map<string,string> mymap;
    unordered_map<string,string>::const_iterator it;
    string mstr = "";
    
    while( y1!= code1y && y2 != code2y){
        if(code1[y1][x1] == code2[y2][x2]){
            // cout << x1 << "/" << x2<<"/" << y1<<"/" << y2 << "  x1/x2/y1/y2 matched "<<endl;
            cout << "   " << code1[y1][x1] << code2[y2][x2]<<endl;
            
            if( isletter(code1[y1][x1]) ){
                mstr.append(code1[y1].substr( x1,1) );
            }else{
                cout << "ELSELESE" << endl;
                it = mymap.find(mstr);
                if( it != mymap.end()){//existing token
                    if(it->first != it->second){
                        int start = x1-mstr.size();
                        test1[y1].erase(start,mstr.size());
                        //cout << "erase "<< test1[y1] << endl;
                        test1[y1].insert(start,it->second);
                        
                        test2[y2].erase(start,mstr.size());
                        //cout << "erase "<< test1[y1] << endl;
                        test2[y2].insert(start,it->second);
                    }
                }else{
                    mymap.insert(make_pair(mstr, mstr));
                }
                mstr = "";
                
            }
            
            x1++;
            x2++;
            if( x1 >= code1[y1].size() ){
                x1 = 0;
                y1++;
            }
            if(x2 >= code2[y2].size()){
                x2 = 0;
                y2++;
            }
        }else if( code1[y1][x1] != code2[y2][x2] && !isalpha(code1[y1][x1]) && !isalpha(code2[y2][x2]) ){
            x1++;
            x2++;
            if( x1 >= code1[y1].size() ){
                x1 = 0;
                y1++;
            }
            if(x2 >= code2[y2].size()){
                x2 = 0;
                y2++;
            }
        }else if( code1[y1][x1] != code2[y2][x2] && !isdigit(code1[y1][x1]) && !isdigit(code2[y2][x2]) ){
            cout << x1 << "/" << x2<<" " << y1<<"/" << y2 << "  x1/x2/y1/y2 parse "<<endl;
            cout << "  " << code1[y1][x1] << " : " << code2[y2][x2]<<endl;
            cout << "       " << test1[y1] << " : " << test2[y2] << endl;
            s1 = s2 = "";
            while(  x1 < code1[y1].size() &&  isletter(code1[y1][x1]) ){
                s1.append(code1[y1].substr( x1,1) );
                if(x1 == code1[y1].size()-1)
                    break;
                x1++;
                if( x1 >= code1[y1].size() ){
                    x1 = 0;
                    y1++;
                }
                if(y1 == code1.size() ){
                    y1--;
                    break;
                }
            }
            
            while(x2 < code2[y2].size()&&  isletter(code2[y2][x2]) ){
                s2.append(code2[y2].substr( x2,1) );
                if(x2 == code2[y2].size()-1)
                    break;
                x2++;
                if(x2 >= code2[y2].size()){
                    x2 = 0;
                    y2++;
                }
                if(y2 == code2.size() ){
                    y2--;
                    break;
                }
            }
            
            cout << "s1/s2 " << s1 << "/"<<s2 << endl;
            cout << "x1/x2/y1/y2 " << x1 << "/"<<x2<< "/"<<y1 << "/"<<y2 << endl;
            //it = mymap.find(s1);
            it = mymap.find(s2);//replacing code1 with code2
            if(it == mymap.end() ){
                mymap.insert(make_pair(s2,s1));
                int mark = x2-s2.size();
                if(x2 == code2[y2].size()-1 && y2 == code2.size()-1)
                    mark++;
                cout << "pre "<< mark <<" " <<test1[y1] << endl;
                test1[y1].erase(mark,s1.size());
                cout << "erase "<< test1[y1] << endl;
                test1[y1].insert(mark, s2);
                cout << "post "<< test1[y1] << endl;
                //x1 = x1-s1.size();
                //x1 += s1.size();
            }else{
                cout <<"mapping found " <<s2 <<" for " << it->second << endl;
                int mark = x2-s2.size();
                if(x2 == code2[y2].size()-1 && y2 == code2.size()-1)
                    mark++;
                if(s2 != it->second ){
                    cout << "pre "<< mark <<" "<< test1[y1] << endl;
                    test1[y1].erase(mark,s1.size());
                    cout << "erase "<< test1[y1] << endl;
                    test1[y1].insert(mark, it->first);
                    cout << "post "<< test1[y1] << endl;
                }
                //x1 = x1-s1.size();
                //x1 += s1.size();
            }
            it = mymap.find(s1);//viceversa
            if(it == mymap.end() ){
                mymap.insert(make_pair(s1,s2));
                int mark = x1-s1.size();
                if(x1 == code1[y1].size()-1 && y1 == code1.size()-1)
                    mark++;
                cout << "pre "<< mark <<" "<< test2[y2] << endl;
                test2[y2].erase(mark,s2.size());
                cout << "erase "<< test2[y2] << endl;
                test2[y2].insert(mark, s1);
                cout << "post "<< test2[y2] << endl;
                //x2 = x2-s2.size();
                //x2 += s2.size();
            }else{
                cout <<"mapping found " <<s1 <<" for " << it->second << endl;
                int mark = x1-s1.size();
                if(x1 == code1[y1].size()-1 && y1 == code1.size()-1)
                    mark++;
                if(s1 != it->second ){
                    cout << "pre "<< mark <<" "<< test2[y2] << endl;
                    test2[y2].erase(mark,s2.size());
                    cout << "erase "<< test2[y2] << endl;
                    test2[y2].insert(mark, it->first);
                    cout << "post "<< test2[y2] << endl;
                }
                //x2 = x2-s2.size();
                //x2 += s2.size();
            }
            
            if(y1+1 <= code1.size() && x1 >= code1[y1].size() ){
                x1 = 0;
                y1++;
            }
            if(y2+1 <= code2.size() &&x2 >= code2[y2].size()){
                x2 = 0;
                y2++;
            }
            if( ( (x1 == code1[y1].size()-1 && y1 == code1.size()-1))&&(x2 == code2[y2].size()-1 && y2 == code2.size()-1)  )
                break;
        }else{
            cout << "infinite else " << endl;
            cout << x1 << "/" << x2<<" " << y1<<"/" << y2 << "  x1/x2/y1/y2 "<<endl;
            cout << "   " << code1[y1][x1] << code2[y2][x2]<<endl;
        }
        
    }
    
    cout <<"FINAL COUNTS  " << x1 << "/" << x2<<" " << y1<<"/" << y2 << "  x1/x2/y1/y2 "<<endl;
    if( code1.size() != code2.size())
        return false;
    int count = 0;
    for(int i = 0; i < code1.size(); i++){
        cout << code1[i] << " - " << code2[i] << endl;
        cout << test1[i] << " - " << test2[i] << endl;
        if(code1[i] == test2[i] || code2[i] == test1[i] ){
            count++;
        }
        
    }
    cout << "MAP STUFF  " << endl;
    for ( it = mymap.begin(); it != mymap.end(); it++ )
    {
        std::cout << it->first  // string (key)
        << ':'
        << it->second   // string's value
        << std::endl ;
    }
    cout <<  "count check "<<count << endl;
    if(count == code1.size())
        return true;
    else
        return false;
}
