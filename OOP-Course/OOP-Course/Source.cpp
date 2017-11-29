#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <list>
using namespace std;

enum Communication{water,electricity,phone,path,canal};
const int sizeofc = 5;

class CEstate {
protected:
     string address;
     string owner;
     long double price;
     double sqft;
     string location;
public:
     CEstate() {
          address = "null"; owner = "null"; price = 0.0; sqft = 0.0; location = "null";
     }
     CEstate(const string& ad, const string& own, const long double pr, const double sq,const string& loc)
     {
          address = ad; owner = own; price = pr; sqft = sq; location = loc;
     }
     CEstate(const CEstate& C) { address = C.address; owner = C.owner; price = C.price; sqft = C.sqft; location = C.location; }
     void setaddr(const string& ad) { address = ad; }
     void setowner(const string& own) { owner = own; }
     void setprice(const long double pr) { price = pr; }
     void setsqft(const double sq) { sqft = sq; }
     void setloc(const string& loc) { location = loc; }
     string getaddr() const { return address; }
     string getown() const { return owner; }
     double getprice() const { return price; }
     double getsqft() const { return sqft; }
     string getloc() const { return location; }
     friend ostream& operator<<(ostream& output, const CEstate& C)
     {
          output << C.address << " " << C.owner <<" "<<C.price<<" "<<C.sqft<<" "<<C.location<< endl;
          return output;
     }
     virtual void scan(istream& input)
     {
          input >> address >> owner >> price >> sqft >> location;
     }
     friend istream& operator>>(istream& input, CEstate& C)
     {
          C.scan(input);
          return input;
     }
     virtual void print()
     {
          cout <<"Address:"<< address << " Owner:" << owner << " Price:" << price << " Sqft:" << sqft <<" Location:"<<location<< endl;
          cout << "------------------------------------------" << endl;
     }
     bool operator<(CEstate& C)
     {
          return(price<C.price);
     }
     bool operator==(const CEstate& C)
     {
          return(sqft == C.sqft);
     }
};

class CFlat :public CEstate {
private:
     int rooms;
     int floor;
public:
     CFlat() { CEstate(); rooms = 0; floor = 0; }
     CFlat(const string& ad, const string& own, const double pr, const double sq,const string& loc, const int r, const int f) :CEstate(ad, own, pr, sq,loc) { rooms = r; floor = f; }
     CFlat(const CEstate& C, const CFlat& F) :CEstate(C){ rooms = F.rooms; floor = F.floor; }
     void setrooms(const int r) { rooms = r; }
     void setfloor(const int f) { floor = f; }
     int getrooms() const { return rooms; }
     int getfloor() const { return floor; }
     friend ostream& operator<<(ostream& output, const CFlat& C)
     {
          output << C.address << " " << C.owner << " " << C.price << " " << C.sqft << " " << C.location << endl;
          output << C.rooms << " " << C.floor << endl;
          return output;
     }
     void scan(istream& input)
     {
          CEstate::scan(input);
          input >> rooms >> floor;
     }
     friend istream& operator>>(istream& input, CFlat& C)
     {
          C.scan(input);
          return input;
     }
     void print()
     {
          cout << "\tFlat:" << endl;
          CEstate::print();
          cout << "Number of rooms:" << rooms << " Floor:" << floor <<endl;
     }
};

class CHouse :public CEstate
{
private:    
     int rooms;
     int floors;
     double sq_yard;
public:
     CHouse() { CEstate(); rooms = 0; floors = 0; sq_yard = 0.0; }
     CHouse(const string& ad, const string& own, const double pr, const double sq,const string& loc, const int r, const int f, const double y) :CEstate(ad, own, pr, sq,loc) { rooms = r; floors = f; sq_yard = y; }
     CHouse(const CEstate& C, const CHouse& H) :CEstate(C){ rooms = H.rooms; floors = H.floors; sq_yard = H.sq_yard; }
     void setrooms(const int r) { rooms = r; }
     void setfloors(const int f) { floors = f; }
     void setsqyard(const double y) { sq_yard = y; }
     int getrooms() const { return rooms; }
     int getfloors() const { return floors; }
     double getsqyard() const { return sq_yard; }
     friend ostream& operator<<(ostream& output, const CHouse& C)
     {
          output << C.address << " " << C.owner << " " << C.price << " " << C.sqft << " " << C.location << endl;
          output << C.rooms << " " << C.floors <<" "<<C.sq_yard<< endl;
          return output;
     }
     void scan(istream& input)
     {
          CEstate::scan(input);
          input >> rooms >> floors >> sq_yard;
     }
     friend istream& operator>>(istream& input, CHouse& C)
     {
          C.scan(input);
          return input;
     }
     void print()
     {
          cout << "\tHouse:" << endl;
          CEstate::print();
          cout << "Number of rooms:" << rooms << " Number of floors:" << floors <<" Sq of yard:"<<sq_yard<<endl;
     }
};

class CPlot : public CEstate {
private:
     list<Communication> available;
public:
     CPlot() { CEstate(); }
     CPlot(const string& ad, const string& own, const double pr, const double sq,const string& loc, const list<Communication>& av) :CEstate(ad, own, pr, sq,loc) { available = av; }
     CPlot(const CEstate& C, const CPlot& P) :CEstate(C){ available = P.available; }
     void setC(const list<Communication>& av){ available = av; }
     string getC() const {
          string av;
          auto it = available.begin();
          int i = 0;
          bool w=false, e=false, p=false, c=false, ph = false;
          while (it != available.end())
          {
               if (*it == water && w==false) {
                    av += "|water|"; w = true;
               }
               else if (*it == electricity && e == false) {
                    av += "|electricity|"; e = true;
               }
               else if (*it == path && p == false) {
                    av += "|path|"; p = true;
               }
               else if (*it == canal && c == false) {
                    av += "|canal|"; c = true;
               }
               else if (*it == phone && ph == false){
                    av += "|phone|"; ph = true;
               }
               else cout << "Nonexistent communication!" << endl;
               it++;
          }
          return av;
     }
     friend ostream& operator<<(ostream& output, const CPlot& C)
     {
          output << C.address << " " << C.owner << " " << C.price << " " << C.sqft << " " << C.location << endl;
          output << C.getC() << endl;
          return output;
     }
     void scan(istream& input)
     {
          CEstate::scan(input);
          int size = 0;
          string str;
          auto it = available.begin();
          input >> size;
          for (int i = 0; i < size; i++)
          {
               input >> str;
               if (str == "water") available.insert(it, water);
               else if (str == "electricity") available.insert(it, electricity);
               else if (str == "canal") available.insert(it, canal);
               else if (str == "path") available.insert(it, path);
               else if (str == "phone") available.insert(it, phone);
               else throw "Wrong value!";
          }
     }
     friend istream& operator>>(istream& input, CPlot& C)
     {
          C.scan(input);
          return input;
     }
     void print()
     {
          cout << "\tPlot:" << endl;
          CEstate::print();
          cout << "Communications:" <<getC() << endl;
     }
};

struct isValueSqft
{
     int m_value;

     isValueSqft(int value) : m_value(value) {}

     bool operator()(const CEstate *cls) const
     {
          return (cls->getsqft() == m_value);
     }
};
bool compare(CEstate* a, CEstate* b) { return (*a < *b); }
struct isValuePrice
{
     int m_value1;
     int m_value2;
     isValuePrice(int value1, int value2) {m_value1 = value1; m_value2 = value2; }

     bool operator()(const CEstate *cls) const
     {
          return (cls->getprice() >= m_value1&&cls->getprice()<=m_value2 );
     }
};

class CRealEstate {
private:
     vector<CEstate*> estates;
public:
     CRealEstate(const string& Filename)
     {
          ifstream fp;
          fp.open(Filename);
          try {
               if (!fp) throw "File nonexistent.";
               else {
                    while (!fp.eof())
                    {
                         int ch;
                         fp >> ch;
                         CEstate *ptr;
                         switch (ch)
                         {
                         case 1:
                             
                              ptr = new CFlat ();
                              fp >> *ptr;
                              estates.push_back(ptr);
                              break;
                         case 2:
                              ptr = new CHouse();
                              fp >> *ptr;
                              estates.push_back(ptr);
                              break;
                         case 3:
                              ptr = new CPlot();
                              fp >> *ptr;
                              estates.push_back(ptr);
                              break;

                         }
                    }
                    if (fp.bad()) {
                         throw "Reading error.";
                    }
               }
               fp.close();
          }
          catch (char* str) {
               cout << str << endl;
          }
     }
     void SearchBySqft(double sqft)
     {
          /*auto it = estates.begin();
               auto it = find_if(estates.begin(), estates.end(), isValueSqft(sqft));       //Aлтернативен вариант
               if (it != estates.end())
               {
                    (*it)->print();
               }
               else cout << "Item not found!" << endl;
               it++;

          */
          auto it = estates.begin();
          while (it != estates.end())
          {
               if ((*it)->getsqft() == sqft) (*it)->print();
               it++;
          }
         
     }
     void SearchByPriceRange(long int p1, long int p2)
     {
          /*
               auto it = find_if(estates.begin(), estates.end(), isValuePrice(p1,p2));     //Aлтернативен вариант
               if (it != estates.end())
               {
                    (*it)->print();
               }
               else cout << "Item not found!" << endl;
               it++;
          */
          auto it = estates.begin();
          while (it != estates.end())
          {
               if ((*it)->getprice() >= p1 && (*it)->getprice() <= p2)
               {
                    (*it)->print();
               }
               it++;
          }
     }
     void SearchByType(string type)
     {
          auto it = estates.begin();
          string s = "class " + type;
          while (it != estates.end())
          {
               if (typeid(**it).name() == s) (*it)->print();
               it++;
          }
     }
    
     void DisplaySorted(string type)
     {
          sort(estates.begin(), estates.end(),compare);
          reverse(estates.begin(), estates.end());
          auto it = estates.begin();
          string s = "class "+type;
          while (it != estates.end())
          {
               if (typeid(**it).name() == s) (*it)->print();
               it++;
          }
     }
     void AddEstate(CEstate* toAdd)
     {
          estates.push_back(toAdd);
     }
   
     void DeleteEstate(CEstate* toDel)
     {
          estates.erase(remove(estates.begin(),estates.end(),toDel),estates.end());
     }
   
     void print()
     {
          for (int i = 0; i < estates.size(); i++)
          {
               estates[i]->print();
          }
     }
     list<CEstate*> SearchLocation(string loc) {
          list<CEstate*> s;
          auto list_it = s.begin();
          auto it = estates.begin();
          while (it != estates.end())
          {
               if ((*it)->getloc() == loc)
               {
                    s.insert(list_it, (*it));
                    (*it)->print();
               }
               it++;
          }
          return s;
     }

};

int main()
{
     CEstate c1("Studentska - 1", "Mario Zhekov", 60263.2532, 465.21,"Haskovo");
     list<Communication> l1 = { water, electricity, path };
     CPlot p1("Studentska - 1", "Mario Zhekov", 120234.6326, 465.21,"Pleven", l1);
     CHouse h1("Studentska - 2", "Konstantin Georgiev", 60263.2532, 465.21,"Plovdiv", 2, 2, 263.673);
     CFlat f1("Studentska - 3", "Jivko Georgiev", 60263.2532, 465.21,"Pazardjik", 2, 2);
     p1.print();
     cout << endl;
     h1.print();
     cout << endl;
     f1.print();
     cout << endl;
     cout << p1 << endl;
     cout << h1 << endl;
     cout << f1 << endl;
     CPlot p2;
     CHouse h2;
     CFlat f2;
     /*try {                          //Въвеждане от оператор за вход
          cin >> f2;
          cin >> p2;
          cin >> h2;
     }
     catch (char* c) { cout << c << endl; }
     catch (int n) { cout << n << endl; }
     catch (...) { cout << "Def.err" << endl; }
     p2.print();
     cout << endl;
     f2.print();
     cout << endl;
     h2.print();
     cout << endl;
     */
     cout << endl << endl;
     try {
          CRealEstate r1("input.txt");
          r1.print();
          cout << endl << endl;
          r1.SearchBySqft(465);
          r1.SearchBySqft(257);
          r1.SearchBySqft(1111);
          r1.SearchByPriceRange(100000, 200000);
          r1.SearchByType("CHouse");
          cout << endl << endl;
          r1.DisplaySorted("CPlot");
          cout << endl << endl;
          CHouse* d1 = new CHouse("Studentska - 2", "Konstantin", 602436, 465.21,"Bansko", 2, 5, 263.673);
          r1.AddEstate(d1);
          cout << endl << "Adding d1:" << endl;
          r1.print();
          cout << endl << "Deleting d1:" << endl;
          r1.DeleteEstate(d1);
          r1.print();
          cout << endl << endl;
          r1.SearchLocation("Sofia");
          cout << "---------------------Test 1------------------" << endl << endl << endl;
     }
     catch (char* c) { cout << c << endl; }
     catch (int n) { cout << n << endl; }
     catch (...) { cout << "Def.err" << endl; }
     try {
          CRealEstate r2("input_test.txt");
          r2.print();
          cout <<endl<< "Search by sqft-263.43:" << endl;
          r2.SearchBySqft(263.43);
          cout << endl << "Search by price range (100 000:200 000):" << endl;
          r2.SearchByPriceRange(100000, 200000);
          cout << endl << "Search by type of estate (CPlot):" << endl;
          r2.SearchByType("CPlot");
          cout << endl << "Add estates (CPlot,CHouse,CFlat):" << endl;
          /*CHouse* d2 = new CHouse("Studentska - 2", "Konstantin", 602436, 465.21, "Bansko", 2, 5, 263.673);
          CFlat*  f2 = new CFlat("Studentska - 3", "Gabriel", 524123, 325.23, "Sofia", 2, 3);
          CPlot*  p2 = new CPlot("Studentska - 4", "Karina", 512531, 362.21, "Lozenec", l1);
          r2.AddHouse(d2);
          r2.AddPlot(p2);
          r2.AddFlat(f2); 
          r2.print();
          cout << endl << "Delete estates (CPlot,CHouse,CFlat):" << endl;
          r2.DeleteHouse(d2);
          r2.DeletePlot(p2);
          r2.DeleteFlat(f2);
          r2.print();
          */
          CHouse* e1 = new CHouse("Studentska - 2", "Konstantin", 602436, 465.21, "Bansko",2,5,263);
          r2.AddEstate(e1);
          cout << endl << "Adding e1:" << endl;
          r2.print();
          cout << endl << "Deleting e1:" << endl;
          r2.DeleteEstate(e1);
          r2.print();
          cout << endl << "Listed by Location(Sofia):" << endl;
          list<CEstate*> sp = r2.SearchLocation("Sofia");
          cout << endl << "Sorted by price(Descending)-CPlot:" << endl;
          r2.DisplaySorted("CPlot");
     }
     catch (char* c) { cout << c << endl; }
     catch (int n) { cout << n << endl; }
     catch (...) { cout << "Def.err" << endl; }
     system("pause");
     return 0;

}