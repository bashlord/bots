struct quest{
    int h;
    int p;
    double r;
    quest(int h, int p) : h(h), p(p) { this->r = (1.0*p)/h; }
    
    bool operator < (const quest& str) const
    {
        return (r > str.r);
    }
};

int questEfficiencyItem(std::vector<int> h, std::vector<int> points, int timeForQuests) {
    vector<int> optimals;
    vector<quest> list1;
    cout << 1<< endl;
    
    for(int i = 0; i < h.size(); i++)
        list1.push_back( quest( h[i],points[i] ) );
    
    printf("list1 size: %d\n", list1.size());
    sort( list1.begin(), list1.end() );
    cout << "----=====postsort=====---" << endl;
    for(int i = 0; i < h.size(); i++)
        cout << "p h r: "<< list1[i].p << " " << list1[i].h << " "<< list1[i].r <<  endl;
    cout << "----==============---" << endl << endl;
    
    // i need a priority queue
    
    for(int j = 0; j < list1.size(); j++ ){
        int r,rtq;
        if( timeForQuests - list1[j].h >= 0){
            r = list1[j].p;
            rtq = timeForQuests - list1[j].h;
        }else
            continue;
        
        for(int i = 0; i < list1.size(); i++ ){
            if(i!=j){cout << "p h r: "<< list1[i].p << " " << list1[i].h << " "<< list1[i].r <<  endl;
                if( rtq-list1[i].h >= 0 ){
                    r += list1[i].p;
                    rtq-=list1[i].h;
                }
            }
        }
        optimals.push_back(r);
    }
    /*
     while( timeForQuests-list1[i].h >= 0){
     cout << "p h r: "<< list1[i].p << " " << list1[i].h << " "<< list1[i].r <<  endl;
     r += list1[i].p;
     timeForQuests-=list1[i].h;
     i++;
     cout << timeForQuests << endl;
     }*/
    sort(optimals.begin(), optimals.end());
    for(int i = 0; i < optimals.size(); i++){
        cout << optimals[i] << " ";
    }cout <<endl;
    if(optimals.size() == 0)
        return 0;
    return optimals[optimals.size()-1];
}

