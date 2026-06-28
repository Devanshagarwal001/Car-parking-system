#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

enum VehicleType { BIKE=1, CAR, TRUCK };

string typeToString(VehicleType t){
    if(t==BIKE) return "Bike";
    if(t==CAR) return "Car";
    return "Truck";
}

double ratePerHour(VehicleType t){
    if(t==BIKE) return 15;
    if(t==CAR) return 30;
    return 60;
}

string formatDateTime(time_t t){
    char buf[100];
    strftime(buf,sizeof(buf),"%A  %d-%m-%Y  %I:%M %p",localtime(&t));
    return string(buf);
}

class Vehicle {
public:
    string number;
    string owner;
    VehicleType type;
    int hours;
    time_t entryTime;

    Vehicle(string n="", string o="", VehicleType t=CAR, int h=1){
        number=n; owner=o; type=t; hours=h;
        entryTime=time(nullptr);
    }
};

class ParkingSystem{
    vector<Vehicle> list;
    int capacity;

public:
    ParkingSystem(int cap=20){capacity=cap; load();}

    void park(){
        if(list.size()>=capacity){cout<<"Parking Full!\n"; return;}
        string num,own; int t,h;
        cout<<"Vehicle Number: "; cin>>num;
        cout<<"Owner Name: "; cin>>own;
        cout<<"1.Bike 2.Car 3.Truck : "; cin>>t;
        cout<<"Enter Parking Hours: "; cin>>h;
        if(h<1) h=1;
        list.push_back(Vehicle(num,own,(VehicleType)t,h));
        cout<<"Parked Successfully at: "<<formatDateTime(list.back().entryTime)<<"\n";
    }

    void remove(){
        string num; cout<<"Enter Number: "; cin>>num;
        for(int i=0;i<list.size();i++){
            if(list[i].number==num){
                double fee=list[i].hours*ratePerHour(list[i].type);

                cout<<"\n------ RECEIPT ------\n";
                cout<<"Vehicle: "<<list[i].number<<"\n";
                cout<<"Type: "<<typeToString(list[i].type)<<"\n";
                cout<<"Entry: "<<formatDateTime(list[i].entryTime)<<"\n";
                cout<<"Hours: "<<list[i].hours<<"\n";
                cout<<"Amount: Rs "<<fee<<"\n";
                cout<<"---------------------\n";

                list.erase(list.begin()+i);
                return;
            }
        }
        cout<<"Vehicle Not Found!\n";
    }

    void display(){
        if(list.empty()){cout<<"No Vehicles Parked\n";return;}
        cout<<left<<setw(15)<<"Number"<<setw(10)<<"Type"<<setw(10)<<"Hours"<<"Date & Time"<<"\n";
        for(auto &v:list){
            cout<<setw(15)<<v.number<<setw(10)<<typeToString(v.type)<<setw(10)<<v.hours<<formatDateTime(v.entryTime)<<"\n";
        }
    }

    void save(){
        ofstream f("data.txt");
        for(auto &v:list)
            f<<v.number<<" "<<v.owner<<" "<<v.type<<" "<<v.hours<<" "<<v.entryTime<<"\n";
    }

    void load(){
        ifstream f("data.txt");
        if(!f) return;
        Vehicle v; int t,h; time_t et;
        while(f>>v.number>>v.owner>>t>>h>>et){
            v.type=(VehicleType)t; v.hours=h; v.entryTime=et;
            list.push_back(v);
        }
    }
};

int main(){
    ParkingSystem ps;
    int ch;
    do{
        cout<<"\n==== CAR PARKING MANAGEMENT SYSTEM ====\n";
        cout<<"1.Park Vehicle\n2.Remove Vehicle\n3.Show Parking\n4.Exit\n";
        cin>>ch;
        switch(ch){
            case 1: ps.park(); break;
            case 2: ps.remove(); break;
            case 3: ps.display(); break;
            case 4: ps.save(); cout<<"Saved!\n"; break;
            default: cout<<"Invalid\n";
        }
    }while(ch!=4);
}