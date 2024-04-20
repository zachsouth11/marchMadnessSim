#include <iostream>
#include <string>
#include <random>

using namespace std;

struct Team{
    string teamName;
    int seed;
};
vector<Team> Actual32;
vector<Team> Predicted32;
struct stats{
    string teamName;
    int seed;
    int champCount;
    int finalsCount;
    int finalFourCount;
};

struct rd32Stats{
    int tourneyNum;
    int numCorrect;
};
vector<rd32Stats> rd32stats;
vector<Team> north;
vector<Team> south;
vector<Team> east;
vector<Team> west;
const bool print = true;
const int numBrackets = 4;
const int HEADS = 0;
const int TAILS = 1;
void printTeam(Team team){
    int spaces = 25;
    spaces -= team.teamName.length();
    spaces--;
    if(team.seed > 9) spaces--;
    if(print) cout<<team.teamName<<" "<<team.seed;
    if(print) {
        for(int i =0; i < spaces; i++) cout << " ";
    }
}

void setTeams();
void setActual32();
int compareRound32();
void printTeams(vector<Team> teams);
Team predictRegion(vector<Team> teams);

//returns winning team obj
Team playGame(Team awayTeam, Team homeTeam);

int main(){
    srand(time(NULL));
    setTeams();
    setActual32();
    vector<stats> records;
    rd32Stats tmp;
    vector<Team> best32;
    for(int i = 0; i<north.size(); ++i){
        stats makeStats;
        makeStats.finalFourCount=0;
        makeStats.champCount=0;
        makeStats.finalsCount=0;
        makeStats.seed = north[i].seed;
        makeStats.teamName = north[i].teamName;
        records.push_back(makeStats);
        makeStats.seed = south[i].seed;
        makeStats.teamName = south[i].teamName;
        records.push_back(makeStats);
        makeStats.seed = east[i].seed;
        makeStats.teamName = east[i].teamName;
        records.push_back(makeStats);
        makeStats.seed = west[i].seed;
        makeStats.teamName = west[i].teamName;
        records.push_back(makeStats);
    }
    for(int i = 0; i < numBrackets; i++){

    
    while(!Predicted32.empty()) Predicted32.pop_back();
    if(print || i %100000 == 0) cout<<"TOURNEY "<<i<<endl;
    if(print) cout<<"North"<<endl;
    Team northChamp = predictRegion(north);
    if(print) cout<<"South"<<endl;
    Team southChamp = predictRegion(south);
    if(print) cout<<"East"<<endl;
    Team eastChamp = predictRegion(east);
    if(print) cout<<"West"<<endl;
    Team westChamp = predictRegion(west);
    Team awayFinalFourChamp = playGame(northChamp, southChamp);
    Team homeFinalFourChamp = playGame(eastChamp, westChamp);
    Team champion = playGame(awayFinalFourChamp, homeFinalFourChamp);
    if(print) cout<<endl<<"FinalFour"<<endl;
    printTeam(northChamp);
    if(print) cout<<"                                     ";
    printTeam(eastChamp);
    if(print) cout<< endl<<endl<<endl;
    if(print) cout<<"                     ";
    printTeam(champion);
    if(print) cout<<endl<<endl;

    if(print) cout<<"           ";
    printTeam(awayFinalFourChamp);
    if(print) cout<<"              ";
    printTeam(homeFinalFourChamp);
    if(print) cout<<endl<<endl<<endl<<endl<<endl;
    printTeam(southChamp);
    if(print) cout<<"                                     ";
    printTeam(westChamp);
    if(print) cout<<endl;
    
    tmp.tourneyNum = i;
    tmp.numCorrect = compareRound32();
    for(int j = 0; j < records.size(); ++j){
        if(records[j].teamName == northChamp.teamName) records[j].finalFourCount +=1;
        if(records[j].teamName == southChamp.teamName) records[j].finalFourCount +=1;
        if(records[j].teamName == eastChamp.teamName) records[j].finalFourCount +=1;
        if(records[j].teamName == westChamp.teamName) records[j].finalFourCount +=1;
        if(records[j].teamName == awayFinalFourChamp.teamName) records[j].finalsCount +=1;
        if(records[j].teamName == homeFinalFourChamp.teamName) records[j].finalsCount +=1;
        if(records[j].teamName == champion.teamName) records[j].champCount += 1;
    }
        if(rd32stats.size()<20) rd32stats.push_back(tmp);
        else if(tmp.numCorrect > rd32stats[rd32stats.size()-1].numCorrect){
            rd32stats.push_back(tmp);
            bool done = false;
            if(tmp.numCorrect>rd32stats[0].numCorrect) best32 = Predicted32;
            while(!done){
                done = true;
                for(int i = 0; i< rd32stats.size()-1; i++){
                    if(rd32stats[i].numCorrect < rd32stats[i+1].numCorrect){
                        rd32Stats tmp = rd32stats[i];
                        rd32stats[i] = rd32stats[i+1];
                        rd32stats[i+1] = tmp;
                        done=false;
                    }
                }
            }
            rd32stats.pop_back();
        }
    }

    for(int i =0; i<rd32stats.size();i++){
        cout<<rd32stats[i].tourneyNum<<"        "<<rd32stats[i].numCorrect<<endl;
    }

    cout<<endl<<endl<<endl<<"Actual"<<"              "<<"best"<<endl<<endl;
    for(int i = 0;i<Actual32.size(); i++){
        cout<<Actual32[i].teamName;
        for(int j = 0; j < 20 - Actual32[i].teamName.size(); j++) cout<<" ";
        cout<<best32[i].teamName<<endl;
    }
    bool done = false;
    while(!done){
        done = true;
        for(int i = 0; i < records.size()-1; i++){
            if(records[i].champCount < records[i+1].champCount){
                stats tmp = records[i];
                records[i] = records[i+1];
                records[i+1]=tmp;
                done = false;
            }
        }
    }
    cout<<endl<<endl;
    for(int i = 0; i < records.size(); i++){
            cout<<records[i].seed<<"  "<<records[i].teamName<<"    "<< records[i].champCount<<"   "<<records[i].finalsCount<<"    " << records[i].finalFourCount<<endl;
        }
    cout<<endl;
    return 0;
}

int compareRound32(){
    int numCorrect=0;
    if(print) cout<<endl<<endl<<endl<<"Actual"<<"              "<<"Predicted"<<endl<<endl;
    for(int i = 0;i<Actual32.size(); i++){
        if(print){
            cout<<Actual32[i].teamName;
            for(int j = 0; j < 20 - Actual32[i].teamName.size(); j++) cout<<" ";
            cout<<Predicted32[i].teamName<<endl;
        }
        if(Predicted32[i].teamName == Actual32[i].teamName) numCorrect++;
    }
    if(print) cout<<"Num Correct:  "<<numCorrect<<endl;
    if(!print && numCorrect>30){
        for(int i = 0;i<Actual32.size(); i++){
            cout<<Actual32[i].teamName;
            for(int j = 0; j < 20 - Actual32[i].teamName.size(); j++) cout<<" ";
            cout<<Predicted32[i].teamName<<endl;
        }
        cout<<"numCorrect:  "<< numCorrect<<endl<<endl;
    }
    return numCorrect;
}

Team predictRegion(vector<Team> teams){
    //order region
    vector<Team> round64Teams;
    vector<Team> round32Teams;
    vector<Team> round16Teams;
    vector<Team> round8Teams;
    vector<Team> regionChamp;
    round64Teams.push_back(teams[0]);
    round64Teams.push_back(teams[15]);
    round64Teams.push_back(teams[7]);
    round64Teams.push_back(teams[8]);
    round64Teams.push_back(teams[4]);
    round64Teams.push_back(teams[11]);
    round64Teams.push_back(teams[3]);
    round64Teams.push_back(teams[12]);
    round64Teams.push_back(teams[5]);
    round64Teams.push_back(teams[10]);
    round64Teams.push_back(teams[2]);
    round64Teams.push_back(teams[13]);
    round64Teams.push_back(teams[6]);
    round64Teams.push_back(teams[9]);
    round64Teams.push_back(teams[1]);
    round64Teams.push_back(teams[14]);
    //round 64
    for(int i=0; i<round64Teams.size();i+=2){
        round32Teams.push_back(playGame(round64Teams[i], round64Teams[i+1]));
    }
    //round 32
    for(int i=0; i<round32Teams.size();i+=2){
        round16Teams.push_back(playGame(round32Teams[i], round32Teams[i+1]));
    }
    //sweet 16
    for(int i=0; i<round16Teams.size();i+=2){
        round8Teams.push_back(playGame(round16Teams[i], round16Teams[i+1]));
    }
    //elite 8
    for(int i=0; i<round8Teams.size();i+=2){
        regionChamp.push_back(playGame(round8Teams[i], round8Teams[i+1]));
    }
    // print region results
    string space = "    ";
    for(int i = 0; i < round64Teams.size(); i++){
        printTeam(round64Teams[i]);
        if(print) cout<<space;
        if(i<round32Teams.size()){
            Predicted32.push_back(round32Teams[i]);
            printTeam(round32Teams[i]);
            if(print) cout<<space;
        }
        if(i<round16Teams.size()){
            printTeam(round16Teams[i]);
            if(print) cout<<space;
        }
        if(i<round8Teams.size()){
            printTeam(round8Teams[i]);
            if(print) cout<<space;
        }
        if(i<regionChamp.size()){
            printTeam(regionChamp[i]);
            if(print) cout<<space;
        }
        if(print) cout<<endl;
    }

    return regionChamp[0];
}

Team playGame(Team awayTeam, Team homeTeam){
    //away team is top on bracket
    //home team is bottom

    //away
    //vs
    //home

    int awayFlipsLeft;
    int homeFlipsLeft;
    if(awayTeam.seed <= homeTeam.seed){
        awayFlipsLeft = 1;
        if(homeTeam.seed-awayTeam.seed<4) homeFlipsLeft = 1;
        else if(homeTeam.seed-awayTeam.seed<7) homeFlipsLeft = 2;
        else if(homeTeam.seed-awayTeam.seed<10) homeFlipsLeft = 3;
        else if(homeTeam.seed-awayTeam.seed<13) homeFlipsLeft = 4;
        else homeFlipsLeft = 5;
    }
    else{
        homeFlipsLeft = 1;
        if(awayTeam.seed-homeTeam.seed<4) awayFlipsLeft = 1;
        else if(awayTeam.seed-homeTeam.seed<7) awayFlipsLeft = 2;
        else if(awayTeam.seed-homeTeam.seed<10) awayFlipsLeft = 3;
        else if(awayTeam.seed-homeTeam.seed<13) awayFlipsLeft = 4;
        else awayFlipsLeft = 5;
    }
    while(awayFlipsLeft>0 && homeFlipsLeft>0){
        //random coinflip 0 or 1
        
        int random = rand() % 2;
        if(random == HEADS) awayFlipsLeft -= 1;
        else homeFlipsLeft -= 1;
    }
    if (awayFlipsLeft < homeFlipsLeft) return awayTeam;
    return homeTeam;
}

void printTeams(vector<Team> teams){
    for(int i=0;i<teams.size();i++){
        cout<<teams[i].teamName<<"    "<<teams[i].seed<<endl;
    }
}

void setTeams(){
    Team team;
    // for(int i = 0; i < 64; i++){
    //     team.teamName="TN";
    //     team.seed=(i%16)+1;
    //     if(i < 16) north.push_back(team);
    //     else if(i < 32 ) south.push_back(team);
    //     else if(i<48) east.push_back(team);
    //     else west.push_back(team);
    // }
    team.teamName="UConn";
    team.seed=1;
    north.push_back(team);
    team.teamName="Iowa St";
    team.seed=2;
    north.push_back(team);
    team.teamName="Illinois";
    team.seed=3;
    north.push_back(team);
    team.teamName="Auburn";
    team.seed=4;
    north.push_back(team);
    team.teamName="San Diego St";
    team.seed=5;
    north.push_back(team);
    team.teamName="BYU";
    team.seed=6;
    north.push_back(team);
    team.teamName="Washington St";
    team.seed=7;
    north.push_back(team);
    team.teamName="FAU";
    team.seed=8;
    north.push_back(team);
    team.teamName="Northwestern";
    team.seed=9;
    north.push_back(team);
    team.teamName="Drake";
    team.seed=10;
    north.push_back(team);
    team.teamName="Duquesne";
    team.seed=11;
    north.push_back(team);
    team.teamName="UAB";
    team.seed=12;
    north.push_back(team);
    team.teamName="Yale";
    team.seed=13;
    north.push_back(team);
    team.teamName="Morehead St";
    team.seed=14;
    north.push_back(team);
    team.teamName="S Dakota St";
    team.seed=15;
    north.push_back(team);
    team.teamName="Stetson";
    team.seed=16;
    north.push_back(team);

    team.teamName="North Carolina";
    team.seed=1;
    south.push_back(team);
    team.teamName="Arizona";
    team.seed=2;
    south.push_back(team);
    team.teamName="Baylor";
    team.seed=3;
    south.push_back(team);
    team.teamName="Alabama";
    team.seed=4;
    south.push_back(team);
    team.teamName="St Marys";
    team.seed=5;
    south.push_back(team);
    team.teamName="Clemson";
    team.seed=6;
    south.push_back(team);
    team.teamName="Dayton";
    team.seed=7;
    south.push_back(team);
    team.teamName="Mississippi St";
    team.seed=8;
    south.push_back(team);
    team.teamName="Michigan St";
    team.seed=9;
    south.push_back(team);
    team.teamName="Nevada";
    team.seed=10;
    south.push_back(team);
    team.teamName="New Mexico";
    team.seed=11;
    south.push_back(team);
    team.teamName="Grand Canyon";
    team.seed=12;
    south.push_back(team);
    team.teamName="Charleston";
    team.seed=13;
    south.push_back(team);
    team.teamName="Colgate";
    team.seed=14;
    south.push_back(team);
    team.teamName="Long Beach St";
    team.seed=15;
    south.push_back(team);
    team.teamName="HOW/WAG";
    team.seed=16;
    south.push_back(team);

    team.teamName="Houston";
    team.seed=1;
    east.push_back(team);
    team.teamName="Marquette";
    team.seed=2;
    east.push_back(team);
    team.teamName="Kentucky";
    team.seed=3;
    east.push_back(team);
    team.teamName="Duke";
    team.seed=4;
    east.push_back(team);
    team.teamName="Wisconsin";
    team.seed=5;
    east.push_back(team);
    team.teamName="Texas Tech";
    team.seed=6;
    east.push_back(team);
    team.teamName="Florida";
    team.seed=7;
    east.push_back(team);
    team.teamName="Nebraska";
    team.seed=8;
    east.push_back(team);
    team.teamName="Texas A&M";
    team.seed=9;
    east.push_back(team);
    team.teamName="BOIS/COL";
    team.seed=10;
    east.push_back(team);
    team.teamName="NC State";
    team.seed=11;
    east.push_back(team);
    team.teamName="James Madison";
    team.seed=12;
    east.push_back(team);
    team.teamName="Vermont";
    team.seed=13;
    east.push_back(team);
    team.teamName="Oakland";
    team.seed=14;
    east.push_back(team);
    team.teamName="Western KY";
    team.seed=15;
    east.push_back(team);
    team.teamName="Longwood";
    team.seed=16;
    east.push_back(team);

    team.teamName="Purdue";
    team.seed=1;
    west.push_back(team);
    team.teamName="Tennessee";
    team.seed=2;
    west.push_back(team);
    team.teamName="Creighton";
    team.seed=3;
    west.push_back(team);
    team.teamName="Kansas";
    team.seed=4;
    west.push_back(team);
    team.teamName="Gonzaga";
    team.seed=5;
    west.push_back(team);
    team.teamName="South Carolina";
    team.seed=6;
    west.push_back(team);
    team.teamName="Texas";
    team.seed=7;
    west.push_back(team);
    team.teamName="Utah St";
    team.seed=8;
    west.push_back(team);
    team.teamName="TCU";
    team.seed=9;
    west.push_back(team);
    team.teamName="UVA/CSU";
    team.seed=10;
    west.push_back(team);
    team.teamName="Oregon";
    team.seed=11;
    west.push_back(team);
    team.teamName="McNeese";
    team.seed=12;
    west.push_back(team);
    team.teamName="Samford";
    team.seed=13;
    west.push_back(team);
    team.teamName="Akron";
    team.seed=14;
    west.push_back(team);
    team.teamName="St Peters";
    team.seed=15;
    west.push_back(team);
    team.teamName="MTST/GRAM";
    team.seed=16;
    west.push_back(team);

}

void setActual32(){
    Team team;
    team.teamName="Alabama";
    team.seed=1;
    Actual32.push_back(team);
    team.teamName="Maryland";
    team.seed=8;
    Actual32.push_back(team);
    team.teamName="SDSU";
    team.seed=5;
    Actual32.push_back(team);
    team.teamName="Furman";
    team.seed=13;
    Actual32.push_back(team);
    team.teamName="Creighton";
    team.seed=6;
    Actual32.push_back(team);
    team.teamName="Baylor";
    team.seed=3;
    Actual32.push_back(team);
    team.teamName="Missouri";
    team.seed=7;
    Actual32.push_back(team);
    team.teamName="Princeton";
    team.seed=15;
    Actual32.push_back(team);

    team.teamName="Fairleigh";
    team.seed=16;
    Actual32.push_back(team);
    team.teamName="FAU";
    team.seed=9;
    Actual32.push_back(team);
    team.teamName="Duke";
    team.seed=5;
    Actual32.push_back(team);
    team.teamName="Tenessee";
    team.seed=4;
    Actual32.push_back(team);
    team.teamName="Kentucky";
    team.seed=6;
    Actual32.push_back(team);
    team.teamName="Kansas St";
    team.seed=3;
    Actual32.push_back(team);
    team.teamName="Michigan St";
    team.seed=7;
    Actual32.push_back(team);
    team.teamName="Marquette";
    team.seed=2;
    Actual32.push_back(team);

    team.teamName="Houston";
    team.seed=1;
    Actual32.push_back(team);
    team.teamName="Auburn";
    team.seed=9;
    Actual32.push_back(team);
    team.teamName="Miami";
    team.seed=5;
    Actual32.push_back(team);
    team.teamName="Indiana";
    team.seed=4;
    Actual32.push_back(team);
    team.teamName="Pitt";
    team.seed=11;
    Actual32.push_back(team);
    team.teamName="Xavier";
    team.seed=3;
    Actual32.push_back(team);
    team.teamName="Penn St";
    team.seed=10;
    Actual32.push_back(team);
    team.teamName="Texas";
    team.seed=2;
    Actual32.push_back(team);

    team.teamName="Kansas";
    team.seed=1;
    Actual32.push_back(team);
    team.teamName="Arkansas";
    team.seed=8;
    Actual32.push_back(team);
    team.teamName="St Marys";
    team.seed=5;
    Actual32.push_back(team);
    team.teamName="UConn";
    team.seed=4;
    Actual32.push_back(team);
    team.teamName="TCU";
    team.seed=6;
    Actual32.push_back(team);
    team.teamName="Gonzaga";
    team.seed=3;
    Actual32.push_back(team);
    team.teamName="Northwestern";
    team.seed=7;
    Actual32.push_back(team);
    team.teamName="UCLA";
    team.seed=2;
    Actual32.push_back(team);
}

