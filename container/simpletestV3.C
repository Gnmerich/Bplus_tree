 // simpletestV3.h C++11
 // 
 // UE Algorithmen und Datenstrukturen - SS 2016 Universitaet Wien
 // Container - Director's Cut
 // https://cewebs.cs.univie.ac.at/ADS/ss16/
 // 
 // Die Klassen ContainerV3 und ContDynArrayV3 dienen zur Vorbereitung auf das durchzufuehrende Projekt
 // und entsprechen NICHT der endgueltigen Aufgabenstellung des Projektes.
 
 #include <iostream>
 #include <sstream>
 #include <fstream>
 #include <string>
 #include <cstring>
 
 #include "ContainerV3.h"
 #include "ContDynArrayV3.h"
 
 class Person;
 using ElementType = ETYPE;
 
 class Person {
   std::string vorname;
   std::string nachname;
 public:
   bool operator==(const Person& p) const { return vorname == p.vorname && nachname == p.nachname; }
   bool operator>(const Person& p) const { return nachname > p.nachname || (nachname == p.nachname && vorname > p.vorname); }
 
   std::ostream& print(std::ostream& o) const { return o << '[' << nachname << ", " << vorname << ']'; }
   std::istream& read(std::istream& i) { return i >> vorname >> nachname; }
 };
 
 inline std::ostream& operator<<(std::ostream& o, const Person& p) { return p.print(o); }
 inline std::istream& operator>>(std::istream& i, Person& p) { return p.read(i); }
 
 const char* helpstr = 
   "new [n] ........................... create new ContDynArray with initial size n\n"
   "delete ............................ delete Container\n"
   "add <key> [...] ................... add <key>(s) with Container::add(int)\n"
   "remove <key> [...] ................ remove <key>(s) with Container::remove(int)\n"
   "member <key> ...................... call Container::member(<key>)\n"
   "size .............................. call Container::size()\n"
   "empty ............................. call Container::empty()\n"
   "min ............................... call Container::min()\n"
   "max ............................... call Container::max()\n"
   "print ............................. print container with operator<<()\n"
   "apply [asc|desc|dontcare [<n>]] ... traverse container with apply (throw exception in nth call)\n"
   "trace ............................. toggle tracing on/off\n"
   "fadd <filename> ................... add values read from file <filename>\n"
   "fremove <filename> ................ remove values read from file <filename>\n"
   "quit .............................. quit program\n\n"
   "arguments surrounded by [] are optional\n";
 
 bool match(const std::string& s, const char * c) {
   return c && s.length() > 0 && s.length() <= std::strlen(c) && s.compare(0, s.length(), c, s.length()) == 0;
 }
 
 std::istream& operator>>(std::istream& i, OrderV3& o) {
   std::string str;
   i >> str;
   o = match(str, "ascending") ? ascending : match(str, "descending") ? descending : dontcare;
   return i;
 }
 
 class ApplyTestException : public std::exception {
 public:
   virtual const char * what() const noexcept override { return "exception not properly catched in apply()"; }
 };
 
 int main() {
 
   ContainerV3<ElementType>* c = nullptr;
   bool traceIt = false;
   std::cout.setf(std::ios_base::boolalpha);
 
   while (true) {
     if (traceIt) {
       if (c) {
         std::cout << std::endl << "container: " << *c;
       } else {
         std::cout << std::endl << "no container";
       }
     }
     std::cout << std::endl << "> ";
 
     std::string cmdline;
     if (!std::getline(std::cin, cmdline)) break;
 
     std::istringstream cmdstream(cmdline);
     std::string cmd;
 
     cmdstream >> cmd;
 
     try {
       if (cmd.length() == 0) {
       } else if (match(cmd, "quit")) {
         break;
       } else if (match(cmd, "new")) {
         if (c) {
           std::cerr << "container exists, 'delete' it first";
         } else {
           size_t n;
           if (cmdstream >> n)
             c = new ContDynArrayV3<ElementType>(n);
           else
             c = new ContDynArrayV3<ElementType>;
         }
       } else if (match(cmd, "help") || cmd == "?") {
         std::cout << helpstr;
       } else if (match(cmd, "trace")) {
         std::cout << "trace " << ((traceIt = !traceIt) ? "on" : "off");
       } else if (!c) {
         std::cout << "no container (use 'new')";
       } else {
         ElementType key;
         if (match(cmd, "delete")) {
           delete c;
           c = 0;
         } else if (match(cmd, "add")) {
           while (cmdstream >> key) { c->add(key); }
         } else if (match(cmd, "remove")) {
           while (cmdstream >> key) { c->remove(key); }
         } else if (match(cmd, "member")) {
           cmdstream >> key;
           std::cout << "returns " << c->member(key);
         } else if (match(cmd, "size")) {
           std::cout << "returns " << c->size();
         } else if (match(cmd, "empty")) {
           std::cout << "returns " << c->empty();
         } else if (match(cmd, "min")) {
           std::cout << "returns " << c->min();
         } else if (match(cmd, "max")) {
           std::cout << "returns " << c->max();
         } else if (match(cmd, "print")) {
           std::cout << *c;
         } else if (match(cmd, "apply")) {
           int n = 0;
           OrderV3 order;
           cmdstream >> order >> n;
           size_t rc1 = c->apply([&](ElementType e) {
             if (n>0 && !--n) throw ApplyTestException(); // test exception handling of apply()
             std::cout << e << ' ';
           }, order);
           std::cout << "\nreturns " << rc1;
         } else if (match(cmd, "fadd")) {
           std::string filename;
           cmdstream >> filename;
           std::ifstream keystream(filename.c_str());
           while (keystream >> key) { c->add(key); }
         } else if (match(cmd, "fremove")) {
           std::string filename;
           cmdstream >> filename;
           std::ifstream keystream(filename.c_str());
           while (keystream >> key) { c->remove(key); }
         } else {
           std::cout << cmd << "? try 'help'";
         }
       }
     } catch (std::exception& e) {
       std::cout << "catched std::exception " << e.what();
     } catch (...) {
       std::cout << "OOPS! - catched something else";
     }
   }
   return 0;
 }
