d#define MOD 4294967296

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


/*std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}*/

std::vector<std::string> split2(const std::string &s) {
    std::vector<std::string> elems;
    split(s, ' ', std::back_inserter(elems));
    return elems;
}

std::vector<std::string> split3(const std::string &s) {
    std::vector<std::string> elems0,elems1;
    std::vector<std::string> elems_ret(3);
    split(s, ' ', std::back_inserter(elems0));
    //cout << elems0[0] << " " <<elems0[1] <<endl;
    split(elems0[1], ',', std::back_inserter(elems1));
    //cout << elems0[0] << " " <<elems0[1] <<endl;
    //cout << elems1[0] << " " <<elems1[1] <<endl;

    elems_ret[0] = elems0[0];
    elems_ret[1] = elems1[0];
    elems_ret[2] = elems1[1];
    return elems_ret;
}



//vector<unsigned long int> regVals;//(43, 0);
//std::vector< std::vector< int > > instructs;
std::string cpuEmulator(std::vector<std::string> subroutine) {
    bool printflag =true;
    int loopStackCount = 0;
    vector<int> loopStack;
    bool zeroFlag = false;
    vector<unsigned long int> regVals(43, 0);
    vector<int> loopNum(43, 0);
    vector<int> loopBase(43, 0);
    
    vector<vector<int>> addscalars;
    
    // stack made up of 43 spots
    vector<vector<vector<int>>> regmoves;//, std::vector<float> ( 0, 0 ) );
    regmoves.push_back(vector<vector<int>>(43));
    vector<int> lastreg(43,-1);
    vector<vector<pair<int,int>>> dirtyregs;
    //regVals.resize(43,0);
    unordered_map<string,int> mymap;
    unordered_map<string,int>::const_iterator it0;
    unordered_map<string,int>::const_iterator it1;
    
    vector<int> record_instr;
    vector<long int> record_val;
    vector<long int> record_scaler;
    vector<vector<vector<long int>>> record_diff;
    // instructs[0] = flag 
    // instructs[1] = register or a d
    // instructs[2] = extra reg for certain instructs
    std::vector< std::vector< int > > instructs( subroutine.size(), std::vector<int> ( 3, 0 ) );
    //instructs.resize(subroutine.size());
    //for(int i = 0; i < subroutine.size(); i++){
    //    instructs[i].resize ( 3, 0 );
    //}
    unsigned long int r1,r2, one;
    one = 1;
    vector<int> scalar;
    vector<vector<int>> scalervecs;
    scalar.push_back(1);
    for(int i = 0; i < 43; i++){
        string register_name = "R";
        if(i < 10) register_name = "R0";
        mymap.insert(make_pair(register_name+to_string(i), i ));
    }
    int cpu_count = 0;
    for(int i = 0; i < subroutine.size(); i++){
        if(printflag){
        cout << "-----------------------------"<<endl;
        cout << regVals[0]<<" "<< regVals[8]<<" "<<regVals[42]<<endl;
        //cout << regVals[0]<<" "<<regVals[1]<<" "<<regVals[2]<< " "<<regVals[42]<<endl;
        //cout << regVals[0]<<" "<< regVals[1]<<" "<<regVals[2]<<" "<<regVals[42]<<endl;
        //cout << regVals[0]<<" "<< regVals[41]<<" "<<regVals[42]<<endl;
        //cout << regVals[0]<<" "<< regVals[1]<<" "<<regVals[2]<<" "<<regVals[42]<<endl;
        }
        if(zeroFlag == true){
        //cout<< "toggle i to 0"<<endl;
            zeroFlag = false;
            i = 0;
        }
        
        if(printflag){
        cout << "i: ["<< i+1<<"]  "<<"loopStackCount: "<<loopStackCount<<" regmovescount: "<<regmoves.size()<<"  cpucount: "<<cpu_count <<endl;
        cout << " scalars: ";
        for(int o = 0; o < scalar.size(); o++) cout << scalar[o]<<" ";
        cout <<endl;
        cout << "  SUBROUTINE: " << subroutine[i] << endl;}
        if( subroutine[i].length() > 3 ){ //skip NOPs
            if(instructs[i][0] == 0){// has not been preparsed
                if( subroutine[i].at(0) == 'M' || subroutine[i].at(0) == 'A' ){
                    std::vector<std::string> splitted3 = split3(subroutine[i]);
                    //cout << "   "<<splitted3[0] << " " << splitted3[1] << " " << splitted3[2] <<endl;
                    if(subroutine[i].at(0) == 'M'){
                        if( splitted3[1].at(0) == 'R' ){ // MOV Rxx,Ryy ; 1
                            it0 = mymap.find(splitted3[1]);
                            it1 = mymap.find(splitted3[2]);
                            r1 = it0->second;
                            r2 = it1->second;
                            regVals[r2] = regVals[r1];
                            //cout << "MOV Rxx,Ryy"<< endl;
                            //cout << "    regVals["<<r2<<"] = regVals["<<r1<<"] :: "<< regVals[r2]<< "="<< regVals[r1]<<endl;
                            instructs[i][0] = 1;
                            instructs[i][1] = r1;
                            instructs[i][2] = r2;
                            lastreg[instructs[i][2]] = instructs[i][1];
                            loopBase[instructs[i][2]] = loopBase[instructs[i][1]];
                        }else{ // MOV d,Rxx ; 2
                            it1 = mymap.find(splitted3[2]);
                            r1 = std::stoul( splitted3[1] );
                            r2 = it1->second;
                            regVals[r2] = r1;
                            if(printflag){cout << "MOV d,Rxx"<< endl;
                            cout << "    regVals["<<r2<<"] = d :: "<< regVals[r2]<< "="<< r1 << " i="<< i<<endl;}
                            
                            instructs[i][0] = 2;
                            instructs[i][1] = r1;
                            instructs[i][2] = r2;
                        }
                    }else{ // ADD Rxx,Ryy ; 3
                        it0 = mymap.find(splitted3[1]);
                        it1 = mymap.find(splitted3[2]);
                        r1 = it0->second;
                        r2 = it1->second;
                        //cout << "ADD Rxx,Ryy"<< endl;
                        //cout << "    regVals["<<r1<<"] += regVals["<<r2<<"] => "<<regVals[r1] << "+=" <<(regVals[r2])<<endl;
                        regVals[r1] = (regVals[r2]+regVals[r1])%MOD;
                        //cout << "         "<< regVals[r1]<<endl;
    
                        instructs[i][0] = 3;
                        instructs[i][1] = r1;
                        instructs[i][2] = r2;
                        if(r1 == 0 && r1 == r2 && record_instr.size()> 0){
                            vector<int> t;
                            int scal = log2(regVals[r1]);
                            t.push_back(-1);
                            t.push_back(32-scal);
                            addscalars.push_back(t);
                        }else if(r1 == 0 && r1 == lastreg[r2] && loopBase[r1] == loopBase[r2]&& record_instr.size()> 0){
                            vector<int> t;
                            int scal = log2(regVals[r1]);
                            t.push_back(-1);
                            t.push_back(32-scal);
                            addscalars.push_back(t);
                        }
                    }
                }else{
                    std::vector<std::string> splitted2 = split2(subroutine[i]);
                    if( subroutine[i].at(0) == 'J'){
                        r1 = std::stoul( splitted2[1] );
                        if(subroutine[i].at(1) == 'Z'){ // JZ d ; 8
                            if(regVals[0] == 0){
                                if(r1 < 2){ 
                                    instructs[i][0] = 8;
                                    instructs[i][1] = -1;
                                    zeroFlag = true;
                                    i = 0;
                                }else{ 
                                    instructs[i][0] = 8;
                                    instructs[i][1] = r1-2;
                                    i = r1-2;
                                }
                            }else{
                                record_instr.push_back(i);
                                record_val.push_back(regVals[0]);
                                record_scaler.push_back(0);
                                record_diff.push_back(vector<vector<long int>>);
                                instructs[i][0] = 8;
                                loopNum.resize(loopNum.size()+1);
                                //regmoves.resize(regmoves.size()+1);
                                //regmoves.push_back(vector<vector<unsigned long int>>(43) );
                                if(r1 < 2) instructs[i][1] = -1;
                                else instructs[i][1] = r1-2;
                            }
                           
                        }else{                     // JMP d ; 7
                            if( r1 < 2){
                                zeroFlag = true;
                        
                                instructs[i][0] = 7;
                                instructs[i][1] = -1;
                                i = 0;
                            }else{
                                
                                instructs[i][0] = 7;
                                instructs[i][1] = r1-2;
                                i = r1-2;
                            }
   
                        }
                    }else{
                        it0 = mymap.find(splitted2[1]);
                        // get reg index
                        r1 = it0->second;

                        if(splitted2[0] == "DEC" ){     // DEC Rxx ; 4
                            //cout << "DEC regVals["<<r1<<"] = " << regVals[r1]; 
                            regVals[r1] = (regVals[r1]-(one))%MOD;
                            //cout << " |  postDec "<< regVals[r1]<<endl;
                            instructs[i][0] = 4;
                            instructs[i][1] = r1;
                        }else if(splitted2[0] == "INC" ){   //INC d ; 5
                            //cout << "INC regVals["<<r1<<"] = " << regVals[r1]; 
                            regVals[r1] = (regVals[r1]+(one))%MOD;
                            //cout << " |  postINC "<< regVals[r1]<<endl;
                            instructs[i][0] = 5;
                            instructs[i][1] = r1;
                        }else{                    //  INV d ; 6
                            //cout << "INV regVals["<<r1<<"] = " << regVals[r1];
                            regVals[r1] = (~regVals[r1])%MOD;
                            //cout << " |  postINV "<< regVals[r1]<<endl;
                            instructs[i][0] = 6;
                            instructs[i][1] = r1;
                        }
                    }
                }
            }else{
                //cout << "<<<<<<<<<<<<<<<<<<instructs[i][0]/i: "<<instructs[i][0]<<"/"<<i<<endl;
               if(instructs[i][0] == 1 ){ //MOV Rxx,Ryy
                   regVals[instructs[i][2]] = regVals[instructs[i][1]];
                   
                   if( lastreg[instructs[i][2]] != instructs[i][1])loopNum[instructs[i][2]] = loopStackCount;
                   cout << "regmoves[loopStackCount]["<<instructs[i][1]<<"] appended register " << instructs[i][2]<<endl;
                   
                   if(lastreg[instructs[i][2]] != -1){
                   //for(int r = 0; r < regmoves[lastreg[instructs[i][2]]].size(); r++){
                     //  if(regmoves[lastreg[instructs[i][2]]][r] == instructs[i][2]){ 
                           regmoves[loopStackCount][lastreg[instructs[i][2]]].erase(std::remove(regmoves[loopStackCount][lastreg[instructs[i][2]]].begin(), regmoves[loopStackCount][lastreg[instructs[i][2]]].end(),(int)instructs[i][2] ), regmoves[loopStackCount][lastreg[instructs[i][2]]].end());
                       //    }
                       //vec.erase(std::remove(vec.begin(), vec.end(), int_to_remove), vec.end());
                       //regmoves[lastreg[instructs[i][2]]]
                       //}
                   }
                   regmoves[loopStackCount][instructs[i][1]].push_back(instructs[i][2]);
                   loopBase[instructs[i][2]] = loopBase[instructs[i][1]];
                   lastreg[instructs[i][2]] = instructs[i][1];
               }else if(instructs[i][0] == 2 ){ //MOV d,Rxx
                  //if(printflag){ //cout << "~~~~~~~~~~~~~~~~~~~~~"<<endl;
                       //cout << instructs[i][0]<<","<<instructs[i][1]<< " " <<regVals[instructs[i][2]]<<endl;}
                       regVals[instructs[i][2]] = instructs[i][1]%MOD;
                       loopNum[instructs[i][2]] = loopStackCount;
                       if(lastreg[instructs[i][2]] != -1){
                           regmoves[loopStackCount][lastreg[instructs[i][2]]].erase(std::remove(regmoves[loopStackCount][lastreg[instructs[i][2]]].begin(),
                                       regmoves[loopStackCount][lastreg[instructs[i][2]]].end(),instructs[i][2] ), 
                                                           regmoves[loopStackCount][lastreg[instructs[i][2]]].end());
                         }
                       lastreg[instructs[i][2]] = -1;
                       loopBase[instructs[i][2]] = loopStackCount;
                       
                       
                   }else if(instructs[i][0] == 3 ){ //ADD Rxx,Ryy
                   int endscalar0 = 1;
                     int endscalar1 = 1;
                   
                  for(int e = 0; e < scalar.size(); e++){
                       //cout << "endscalar0 *= scalar[e] : "<< endscalar0 <<" *= "<< scalar[e]<<endl;
                           //if(e <= loopStackCount) endscalar0 *= scalar[e];
                           endscalar0 = scalar[loopStackCount];
                           if(e <= loopNum[instructs[i][1]]) endscalar1 *= scalar[e];
                       }
                       
                    if( lastreg[instructs[i][2]] ==  instructs[i][1] ){
                        if(regVals[instructs[i][1]] == 1 && regVals[instructs[i][2]] == 1){
                            endscalar0--;
                            regVals[instructs[i][1]] = 2;
                            regVals[instructs[i][2]] = 2;
                        }
                           regVals[instructs[i][1]] = pow(regVals[instructs[i][1]],endscalar0+1);
                           regVals[instructs[i][2]] = pow(regVals[instructs[i][2]],endscalar0);
                           regVals[instructs[i][1]] = regVals[instructs[i][1]]%MOD;
                           regVals[instructs[i][2]] = regVals[instructs[i][2]]%MOD;
                       
                       for(int r = 0; r < regmoves[loopStackCount][instructs[i][1]].size();r++){
                       //cout << "regmoves[instructs[i][1]][r]="<< regmoves[instructs[i][1]][r]<<endl;
                           if( loopNum[regmoves[loopStackCount][instructs[i][1]][r]]==loopStackCount ) regVals[regmoves[loopStackCount][instructs[i][1]][r]] = regVals[instructs[i][2]];
                       }
                       regmoves[loopStackCount][instructs[i][1]].clear();
                       regmoves[loopStackCount][instructs[i][2]].push_back(instructs[i][1]);
                       loopNum[instructs[i][1]] = loopStackCount;
                   }else if(instructs[i][2] == instructs[i][1] ){
                   if(regVals[instructs[i][1]] == 1 && regVals[instructs[i][2]] == 1){
                            endscalar0--;
                            regVals[instructs[i][2]] = 2;
                        }
                       regVals[instructs[i][2]] = pow(regVals[instructs[i][2]],endscalar0+1);
                       regVals[instructs[i][2]] = regVals[instructs[i][2]]%MOD;
                       unsigned long int pr;
                       for(int r = 0; r < regmoves[loopStackCount][instructs[i][1]].size();r++){
                           pr = pow(regVals[instructs[i][2]],endscalar0);
                           if( loopNum[regmoves[loopStackCount][instructs[i][1]][r]]==loopStackCount ) regVals[regmoves[loopStackCount][instructs[i][1]][r]] = pr%MOD;
                       }
                       regmoves[loopStackCount][instructs[i][1]].clear();
                       loopNum[instructs[i][1]] = loopStackCount;
                       regmoves[loopStackCount][instructs[i][2]].push_back(instructs[i][1]);
                   }else{
                   //cout << "here1"<<endl;
                       regVals[instructs[i][1]] = (regVals[instructs[i][1]]+(regVals[instructs[i][2]]*endscalar0))%MOD;
                       for(int r = 0; r < regmoves[loopStackCount][instructs[i][1]].size();r++){
                           if( loopNum[regmoves[loopStackCount][instructs[i][1]][r]]==loopStackCount ) regVals[regmoves[loopStackCount][instructs[i][1]][r]] = (regVals[instructs[i][1]]+(regVals[instructs[i][2]]*(endscalar0-1)))%MOD;
                       }
                       loopNum[instructs[i][1]] = loopStackCount;
                        regmoves[loopStackCount][instructs[i][2]].push_back(instructs[i][1]);
                        
                    }
                    
               }else if(instructs[i][0] == 4 ){ //DEC Rxx
                      if(printflag){
                      cout<< "DEC Vals: ";
                      cout << "regVals[instructs[i][1]]: " << regVals[instructs[i][1]];
                      cout << ", scalar[loopNum[instructs[i][1]]]: " << scalar[loopNum[instructs[i][1]]];
                      cout << ", loopNum[instructs[i][1]]: " << loopNum[instructs[i][1]];
                      cout << ", loopstackcout: "<< loopStackCount<<endl;
                      }
                     int endscalar0 = 1;
                     int endscalar1 = 1;
                   if(loopStackCount == 0){ regVals[instructs[i][1]] = (regVals[instructs[i][1]]-(one))%MOD;}
                   else{

                       endscalar0 = scalar[loopStackCount];
                       //cout << "endscalars: "<<endscalar0<<", " << endscalar1<<endl;
                       regVals[instructs[i][1]] = (regVals[instructs[i][1]]-(one*endscalar0))%MOD;
                       if(regVals[instructs[i][1]] == MOD ) regVals[instructs[i][1]] = 0;
                       for(int r = 0; r < regmoves[loopStackCount][instructs[i][1]].size();r++){
                       cout << "loopNum[instructs[i][1]]: " << loopNum[instructs[i][1]]<<endl;
                          regVals[regmoves[loopStackCount][instructs[i][1]][r]] = (regVals[instructs[i][1]]+1)%MOD;
                       }
                       regmoves[loopStackCount][instructs[i][1]].clear();
                    }
               }else if(instructs[i][0] == 5 ){ // INC Rxx
                   int endscalar0 = 1;
                     int endscalar1 = 1;
                   if(loopStackCount == 0){ regVals[instructs[i][1]] = (regVals[instructs[i][1]]+(one))%MOD;}
                   else{
                       endscalar0 = scalar[loopStackCount];
                       regVals[instructs[i][1]] = (regVals[instructs[i][1]]+(one*endscalar0))%MOD;
                       
                       for(int r = 0; r < regmoves[loopStackCount][instructs[i][1]].size();r++){
                           if( loopNum[regmoves[loopStackCount][instructs[i][1]][r]]==loopStackCount ) regVals[regmoves[loopStackCount][instructs[i][1]][r]] = (regVals[instructs[i][1]]+1)%MOD;
                       }
                       regmoves[loopStackCount][instructs[i][1]].clear();
                    }
               }else if(instructs[i][0] == 6 ){ //INV Rxx
                   int scalar0 = 1;
                   for(int q = 0; q< scalar.size();q++) scalar0 *= scalar[q];
                   if(scalar0%2 == 1){
                       regVals[instructs[i][1]] = (~regVals[instructs[i][1]]);
                   }else{
                       for(int r = 0; r < regmoves[loopStackCount][instructs[i][1]].size();r++){
                           if( loopNum[regmoves[loopStackCount][instructs[i][1]][r]]==loopStackCount ) regVals[regmoves[loopStackCount][instructs[i][1]][r]] = ~regVals[instructs[i][1]];
                       }
                       regmoves[loopStackCount][instructs[i][1]].clear();
                   
                   }
               }else if(instructs[i][0] == 7 ){ //JMP d
                  if(instructs[i][1] == -1){
                           zeroFlag = true;
                           i = 0;
                       }else{
                           i = instructs[i][1];
                       }
               }else if(instructs[i][0] == 8 ){ //JZ d
                   if( regVals[0]%MOD == 0){
                       if(printflag){for(int p = 0; p<record_instr.size();p++){
                               cout << record_instr[p]<<","<< record_val[p]<<","<<record_scaler[p]<<" ";
                               //if(record_instr[p] == i) prev = p;   
                           }}
                           //if(scalar > 1) scalar = 1;
                           regmoves.pop_back();
                           loopStackCount--;
                           int curr = loopStack[loopStack.size()-1];
                          if(printflag){ cout << "loopStack: " << loopStackCount << "  scalar: "<<endl;

                           cout << " record_instr: (record_val[loopStackCount]/record_scaler[loopStackCount]) =>  record_instr: "<< record_instr[curr]<<"   "<<(record_val[curr])<< "/" << (record_scaler[curr])<<endl;}

                           //scalar /= (record_val[curr]);///record_scaler[loopStackCount]);
                           scalar.pop_back();
                           if(instructs[i][1] == -1){
                               zeroFlag = true;
                               i = 0;
                           }else{
                               i = instructs[i][1];
                           }
                           loopStack.pop_back();
                           if(printflag){cout <<"  HERE loop exit, jump to instr: " <<i<<endl;}

                    }else{
                       if( record_instr.size() > 0){
                       if(printflag){cout << "here ";}//<< record_val[record_val.size()-1] <<" "<< regVals[0]<<" "<<record_instr.size() <<endl;
                       unsigned long int prev;
                       for(int p = 0; p<record_instr.size();p++){
                           if(printflag){cout << record_instr[p]<<","<< record_val[p]<<","<<record_scaler[p]<<" ";}
                           if(record_instr[p] == i){
                           prev = p;   
                           break;
                           }
                       }
                       if(record_diff[prev].size() > 0){
                       
                       }else{
                        int temp = -1;
                           for(int d = 0; d < addscalars.size();d++){
                               if(addscalars[d][0] == -1){
                                   addscalars[d][0] = i;
                                   temp = d;
                                   break;
                               }
                               if(addscalars[d][0] == i){
                                   temp = d;
                                   break;
                               }
                           }
                       if(printflag){cout << "          curr regVals[0]/SIZE/scalar[prev]: " 
                                   <<regVals[0]<<"/"<<record_instr.size()<<"/"<<(record_val[prev]-regVals[0])%MOD<<endl;}
                           
                           if(temp != -1){
                          // cout << "=====loop used ADD scalar====="<<endl;
                               if(record_scaler[prev] == 0) record_scaler[prev] = addscalars[temp][1];
                               record_val[prev] = regVals[0];
                               addscalars[temp][1] = 32-log2(regVals[0]);
                               if(loopStackCount == 0){
                                scalar.push_back(addscalars[temp][1]);
                                regmoves.push_back(vector<vector<int>>(43));
                               loopStackCount = 1;
                           }else{
                                scalar.push_back(addscalars[temp][1]);
                                regmoves.push_back(vector<vector<int>>(43));
                               //scalar *= regVals[0]/record_scaler[prev];
                               loopStackCount++;
                           }
                           loopStack.push_back(prev);
                           }else{
                           // the difference between r00 the prev and curr time looped
                           if(record_scaler[prev] == 0) record_scaler[prev] = (record_val[prev]-regVals[0]);
                           //record_scaler[prev] = 1;
                           if(record_scaler[prev] > 0){
                               record_val[prev] = regVals[0];
                            }else{
                                record_val[prev] = (regVals[0]-MOD);
                            }
                           if(loopStackCount == 0){
                                scalar.push_back(record_val[prev]/record_scaler[prev]);
                               loopStackCount = 1;
                               regmoves.push_back( vector<vector<int>>(43) );
                           }else{
                                scalar.push_back(record_val[prev]/record_scaler[prev]);
                                regmoves.push_back( vector<vector<int>>(43) );
                               loopStackCount++;
                           }
                           loopStack.push_back(prev);
                           }
                           }
                       }else{
                           record_instr.push_back(i);
                           record_val.push_back(regVals[0]);
                       }
                   } 
               }
            
            
            }
        }//else cout << "   NOP" <<endl;
        cpu_count++;
        
        }
    
    
    if(printflag){for(int i = 0; i < 43; i++){
        cout<< i <<": "<< regVals[i] <<endl;
    }}
    
    cout << "total instructions called: "<<cpu_count<<endl;
    return to_string(regVals[42]%MOD);
}

/*
else if(loopStackCount>=2){

                        loopStackCount--;
                           int curr = loopStack[loopStack.size()-1];
                           if(printflag){cout << "loopStack: " << loopStackCount << "  scalar: "<<endl;

                           cout << " record_instr: (record_val[loopStackCount]/record_scaler[loopStackCount]) =>  record_instr: "<< record_instr[curr]<<"   "<<(record_val[curr])<< "/" << (record_scaler[curr])<<endl;}

                           //scalar /= (record_val[curr]/record_scaler[curr]);///record_scaler[loopStackCount]);
                           scalar.pop_back();
                           if(instructs[i][1] == -1){
                               zeroFlag = true;
                               i = 0;
                           }else{
                               i = instructs[i][1];
                           }
                           loopStack.pop_back();
                           regVals[0] = 0;
                          if(printflag){ cout <<"  HERE loop exit, jump to instr: " <<i<<endl;}
                    }
*/

                       /*for(int s = 0; s < scalar; s++){
                       regVals[instructs[i][2]] = regVals[instructs[i][1]];
                           regVals[instructs[i][1]] += (regVals[instructs[i][2]]);
                           regVals[instructs[i][1]] = regVals[instructs[i][1]]%MOD;
                           if(printflag){
                           cout << regVals[instructs[i][1]] <<" "<< regVals[instructs[i][2]]<<endl;}
                       }*/
