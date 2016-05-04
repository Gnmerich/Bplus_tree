 #ifndef CONTDYNARRAYV3_H
 #define CONTDYNARRAYV3_H
 
 // ContDynArrayV3.h C++11
 // 
 // UE Algorithmen und Datenstrukturen - SS 2016 Universitaet Wien
 // Container - Director's Cut
 // https://cewebs.cs.univie.ac.at/ADS/ss16/
 // 
 // Die Klassen ContainerV3 und ContDynArrayV3 dienen zur Vorbereitung auf das durchzufuehrende Projekt
 // und entsprechen NICHT der endgueltigen Aufgabenstellung des Projektes.
 // Der Code in diesen Dateien ist daher NICHT zum Upload in den Unit-Test geeignet.
 
 #include <iostream>
 #include "ContainerV3.h"
 
 class ContDynArrayEmptyExceptionV3 : public ContainerExceptionV3 {
 public:
   virtual const char * what() const noexcept override { return "ContDynArray: empty"; }
 };
 
 template <typename E>
 class ContDynArrayV3 : public ContainerV3<E> {
   size_t nmax;
   size_t n;
   E * values;
   void sort() const;
 public:
   explicit ContDynArrayV3(size_t = 7);
   ContDynArrayV3(std::initializer_list<E> el);
 
   virtual ~ContDynArrayV3() { delete[] values; }
 
   using ContainerV3<E>::add;
   virtual void add(const E e[], size_t len) override;
 
   using ContainerV3<E>::remove;
   virtual void remove(const E e[], size_t len) override;
 
   virtual bool member(const E& e) const override;
   virtual size_t size() const override { return n; }
 
   virtual E min() const override;
   virtual E max() const override;
 
   virtual std::ostream& print(std::ostream& o) const override;

   virtual size_t apply(std::function<void(E)> f, OrderV3 order = dontcare) const override;
 };
 
 template <typename E>
 ContDynArrayV3<E>::ContDynArrayV3(size_t nmax) : nmax{nmax}, n{0}, values{new E[this->nmax]} { }

 template <typename E>
 ContDynArrayV3<E>::ContDynArrayV3(std::initializer_list<E> el) : ContDynArrayV3(el.size()) { for (auto e: el) add(e); }

 template <typename E>
 void ContDynArrayV3<E>::add(const E e[], size_t len) {
   if (n + len > nmax) {
     auto newnmax = nmax;
     E * newvalues = nullptr;
     while (n + len > newnmax) newnmax = (newnmax*12)/10 + 2;
     newvalues = new E[newnmax];
     for (size_t i = 0; i < n; ++i)
       newvalues[i] = values[i];
     delete[] values;
     values = newvalues;
     nmax = newnmax;
   }
   for (size_t i = 0; i < len; ++i)
     if (!member(e[i]))
       values[n++] = e[i];
 }
 
 template <typename E>
 void ContDynArrayV3<E>::remove(const E e[], size_t len) {
   for (size_t i = 0; i < len; ++i) {
     for (size_t j = 0; j < n; ++j) {
       if (values[j] == e[i]) {
         values[j] = values[--n];
         break;
       }
     }
   }
 }
 
 template <typename E>
 bool ContDynArrayV3<E>::member(const E& e) const {
   for (size_t i = 0; i < n; ++i) {
     if (values[i] == e) return true;
   }
   return false;
 }
 
 template <typename E>
 E ContDynArrayV3<E>::min() const {
   if (this->empty()) throw ContDynArrayEmptyExceptionV3();
   E rc = values[0];
   for (size_t i = 1; i < n; ++i) {
     if (rc > values[i]) rc = values[i];
   }
   return rc;
 }
 
 template <typename E>
 E ContDynArrayV3<E>::max() const {
   if (this->empty()) throw ContDynArrayEmptyExceptionV3();
   E rc = values[0];
   for (size_t i = 1; i < n; ++i) {
     if (values[i] > rc) rc = values[i];
   }
   return rc;
 }
 
 template <typename E>
 std::ostream& ContDynArrayV3<E>::print(std::ostream& o) const {
   o << "ContDynArray [ n=" << n << " nmax=" << nmax << " values=";
   for (size_t i = 0; i < n; ++i) o << ' ' << values[i];
   o << " ]";
   return o;
 }
 
 template <typename E>
 size_t ContDynArrayV3<E>::apply(std::function<void(E)> f, OrderV3 order) const {
   size_t rc = 0;
   if (order != dontcare) sort();
   try {
     if (order == descending) {
       for (size_t i = n; i--;) {
         f(values[i]);
         ++rc;
       }
     } else {
       for (size_t i = 0; i < n; ++i) {
         f(values[i]);
         ++rc;
       }
     }
   } catch (...) {}
   return rc;
 }

 template <typename E>
 void ContDynArrayV3<E>::sort() const {
   for (size_t i = 0; i < n; ++i) {
     size_t min = i;
     for (size_t j = i+1; j < n; ++j)
       if (values[min] > values[j]) min = j;
     std::swap(values[min], values[i]);
   }
 }
 
 #endif //CONTDYNARRAYV3_H
