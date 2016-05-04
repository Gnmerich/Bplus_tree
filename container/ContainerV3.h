 #ifndef CONTAINERV3_H
 #define CONTAINERV3_H
 
 // ContainerV3.h C++11
 // 
 // UE Algorithmen und Datenstrukturen - SS 2016 Universitaet Wien
 // Container - Director's Cut
 // https://cewebs.cs.univie.ac.at/ADS/ss16/
 // 
 // Die Klassen ContainerV3 und ContDynArrayV3 dienen zur Vorbereitung auf das durchzufuehrende Projekt
 // und entsprechen NICHT der endgueltigen Aufgabenstellung des Projektes.
 
 #include <iostream>
 #include <functional>
 
 enum OrderV3 { dontcare, ascending, descending };
 class ContainerExceptionV3;
 
 template <typename E>
 class ContainerV3 {
 public:
   ContainerV3& operator=(const ContainerV3&) = delete;
   ContainerV3(const ContainerV3&) = delete;
   ContainerV3() = default;
   
   virtual ~ContainerV3() {}
 
   virtual void add(const E& e) { add(&e, 1); }
   virtual void add(const E e[], size_t len) = 0;
 
   virtual void remove(E& e) { remove(&e, 1); }
   virtual void remove(const E e[], size_t len) = 0;
 
   virtual bool member(const E& e) const = 0;
   virtual size_t size() const = 0;
   virtual bool empty() const { return size() == 0; }
 
   virtual size_t apply(std::function<void(E)> f, OrderV3 order = dontcare) const = 0;
 
   virtual E min() const = 0;
   virtual E max() const = 0;
 
   virtual std::ostream& print(std::ostream& o) const = 0;
 };
 
 template <typename E>
 inline std::ostream& operator<<(std::ostream& o, const ContainerV3<E>& c) { return c.print(o); }
 
 class ContainerExceptionV3 : public std::exception { 
 public:
   virtual const char * what() const noexcept override = 0;
 };

 #endif //CONTAINERV3_H
