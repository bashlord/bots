DATTO


struct job{
    double backdur;// initially set up as the duration, when it is 0 it is done
    double jobtime;
    double initime;
    int index;
    
    job(){}
    job(int backdur, int initime, int index){
        this->backdur = backdur;
        this->initime = initime;
        this->index = index;
        this->jobtime = 0;
    }
    
    void timeInc(int n, int turbo){
        //n is the number of active threads
        if( (this->backdur - ( (turbo*1.0) /n)) < 0 ){//does not require an entire TC
            double t = (this->backdur/((turbo*1.0)/n));
            double tt = (turbo*1.0)/n;
            cout << " ==============time inc==========="<<endl;
            cout <<"t/backdur/tt/jobtime  " <<t <<"/"<<this->backdur << "/" <<  tt <<"/" << this->jobtime<< endl;
            
            this->jobtime += (turbo*1.0)*t;
            this->backdur -= ((turbo*1.0)/n);
        }else{
            this->jobtime += (turbo*1.0);
            this->backdur -= ((turbo*1.0)/n);
        }
    }
    bool done(){
        if(this->backdur == 0)
            return true;
        return false;
    }
    bool done1(){
        if(this->backdur < 0)
            return true;
        return false;
    }
    double val(){
        return this->initime + this->jobtime;
    }
    void p(){
        cout <<"inittime/index/backdur/jobtime:   ";
        cout << this->initime << "/" << this->index << "/" << this->backdur <<"/" << this->jobtime<<endl;
    }
    
    bool operator < (const job& str) const
    {
        return (index < str.index);
    }
    
};

std::vector<double> backupTimeEstimator(std::vector<int> startTimes, std::vector<int> backupDuration, int maxThreads) {
    vector<double> r;
    if(startTimes.size() == 0)
        return r;
    vector<job> q;
    vector<job> waiting;
    vector<job> fin;
    bool mode50 = false;
    bool mode100 = false;
    bool mode1k = false;
    
    bool mode500 = false;
    bool mode750 = false;
    bool mode25 = false;
    bool mode250 = false;
    bool mode10 = false;
    
    
    
    for(int i = 0; i < startTimes.size(); i++)
        waiting.push_back( job(backupDuration[i], startTimes[i], i ) );
    int i = 1;
    q.push_back( waiting[0] );
    int clock = startTimes[0]+1;
    // TC = Time Cycle
    
    while(fin.size() != backupDuration.size()){
        // when jobs take a thread, they do not progress until the next TC
        
        cout << "Current time: " << clock << " i: " << i << endl;
        
        int a = 0;
        cout << "PRE queue: ";
        for(int w = 0; w < q.size(); w++)
            cout << q[w].jobtime << "|" << q[w].backdur << "|" <<q[w].index << " ";
        cout <<endl;
        
        int theSchwartz = 1;
        if(mode1k)
            theSchwartz = 1000;
        else if(mode750)
            theSchwartz = 750;
        else if(mode500)
            theSchwartz = 500;
        else if(mode250)
            theSchwartz = 250;
        else if(mode100)
            theSchwartz = 100;
        else if(mode50)
            theSchwartz = 50;
        else if(mode25)
            theSchwartz = 25;
        else if(mode10)
            theSchwartz = 10;
        bool tag = false;
        int ti = -1;
        int presize = q.size();
        while( a < q.size() ){
            q[a].timeInc(presize, theSchwartz);
            if( q[a].done() ){
                cout << "WE GOT A WINNER " <<endl;
                q[a].p();
                tag = true;
                fin.push_back( q[a] );
                //r.push_back( q[a].val() );
                ti = fin.size()-1;
                q.erase(q.begin() + a);
            }else if( q[a].done1() ){
                cout << "I am so done with this " <<endl;
                q[a].p();
                double wth = q[a].backdur*-1.0;
                tag = true;
                fin.push_back( q[a] );
                //r.push_back( q[a].val() );
                ti = fin.size()-1;
                q.erase(q.begin() + a);
                for(int h = 0; h < q.size(); h++)
                    q[h].backdur -= wth/q.size();
            }else{
                a++;
            }
        }
        
        cout << "-------  jobtime|backdur|index" << endl;
        cout << "pOST queue: ";
        for(int w = 0; w < q.size(); w++)
            cout << q[w].jobtime << "|" << q[w].backdur << " ";
        cout <<endl;
        cout << endl;
        
        if( q.size() < maxThreads && i < startTimes.size() && waiting[i].initime == clock){
            //threads are available for job processing,
            q.push_back( waiting[i]);
            i++;
        }else if(q.size() < maxThreads && waiting[i].initime < clock && tag && i < startTimes.size()){
            waiting[i].initime = fin[ti].val();
            q.push_back( waiting[i]);
            i++;
        }
        clock++;
        
        
        if( i < startTimes.size() ){
            cout << " We need the Schwartz " <<  startTimes[i+1] << " - " << startTimes[i]<< " = " <<startTimes[i+1]-startTimes[i] << endl;
            if(1==1){
                
            }else if( startTimes[i]-startTimes[i-1] >= 1000  ){
                mode1k= true;
                clock += 999;
            }else if(startTimes[i]-startTimes[i-1] >= 750 ){
                mode750 = true;
                clock += 749;
            }else if(startTimes[i]-startTimes[i-1] >= 500 ){
                mode500 = true;
                clock += 499;
            }else if(startTimes[i]-startTimes[i-1] >= 250 ){
                mode250 = true;
                clock += 249;
            }else if(startTimes[i]-startTimes[i-1] >= 100 ){
                mode100 = true;
                clock += 99;
            }else if(startTimes[i]-startTimes[i-1] >= 50 ){
                mode50 = true;
                clock += 49;
            }else if(startTimes[i]-startTimes[i-1] >= 25 ){
                mode25 = true;
                clock += 24;
            }else if(startTimes[i]-startTimes[i-1] >= 10 ){
                mode10 = true;
                clock += 9;
            }else{
                mode1k = mode100 = mode50 =mode750 = mode500 = mode250 = mode100 = mode25 = mode10 = false;
            }
        }
        
    }
    
    sort(fin.begin(), fin.end());
    
    for(int b = 0; b < fin.size(); b++){
        cout << fin[b].jobtime << " ";
        r.push_back(fin[b].val());
    }
    
    return r;
}
