#ifndef MISC_H
#define MISC_H

#include "./include/defines.h"

template <class TYPE>
class Array {
private:
  TYPE * arr;
  int lengthC;
public:
  Array(int length) : arr(new TYPE [length]), lengthC(length) {}
  Array(const Array<TYPE>& ref) : arr(new TYPE [ref.lengthC]), lengthC(ref.lengthC) {for (int i=0,l=ref.lengthC;i<l;i++)this->arr[i]=ref.arr[i];}
  ~Array(){delete [] this->arr;}

  inline int length() const {return this->lengthC;}
  TYPE getData(int index) const {return this->arr[index];}
  void setData(TYPE d, int index){this->arr[index]=d;}

  TYPE& operator[](int index) const {return this->arr[index];}

  Array<TYPE>& operator=(const Array<TYPE>& R){
    delete [] this->arr;
    this->Array(R);
    return *this;
  }

  Array<TYPE>& map(void (*f)(TYPE&)){//changes all values in array basing on value returned by passed function
    for (int i=0,l=this->lengthC;i<l;++i)
      f(this->arr[i]);
    return *this;
  }

  Array<TYPE>& map(TYPE (*f)(const TYPE&)){//allows passed function to change all values in array
    for (int i=0,l=this->lengthC;i<l;++i)
      this->arr[i]=f(this->arr[i]);
    return *this;
  }

  Array<TYPE>& each(void (*f)(const TYPE&)){//calls passed function with all array elements as argument
    for (int i=0,l=this->lengthC;i<l;++i)
      f(this->arr[i]);
    return *this;
  }

  Array<TYPE>& selected(void (*f)(const TYPE&), bool (*p)(const TYPE&)){//calls first passed function only on elements accepted by second one
    for (int i=0,l=this->lengthC;i<l;++i)
      if (p(this->arr[i]))
        f(this->arr[i]);
    return *this;
  }
};

template <class TYPE>
class Chain {
private:
  Chain * nextC;
  Chain * prevC;
public:
  TYPE data;
  Chain(TYPE data) : nextC(nullptr), prevC(nullptr), data(data) {}
  ~Chain(){this->pop();}

  Chain * next(){return this->nextC;}
  Chain * prev(){return this->prevC;}
  Chain * rewind(){Chain<TYPE>*c=this;while(c->prev())c=c->prev();return c;}
  Chain * rewindForward(){Chain<TYPE>*c=this;while(c->next())c=c->next();return c;}

  inline void insertAfter(Chain * c){
    Chain*t=this->nextC;
    this->nextC=c;
    c->prevC=this;
    c=c->rewindForward();
    c->nextC=t;
    if (t)
      t->prevC=c;
  }
  inline void insertBefore(Chain * c){
    Chain*t=this->prevC;
    this->prevC=c;
    c->nextC=this;
    c=c->rewind();
    c->prevC=t;
    if (t)
      t->nextC=c;
  }

  inline void insertAfter(TYPE data){this->insertAfter(new Chain(data));}
  inline void insertBefore(TYPE data){this->insertBefore(new Chain(data));}

  inline void pop(){
    if(this->nextC)
      this->nextC->prevC=this->prevC;
    if(this->prevC)
      this->prevC->nextC=this->nextC;
  }

  inline Chain<TYPE>* map(void (*f)(TYPE&)){
    Chain<TYPE>*c=this->rewind();
    while(c)
      f(c->data);
    return this;
  }

  inline Chain<TYPE>* map(TYPE (*f)(const TYPE&)){
    Chain<TYPE>*c=this->rewind();
    while(c)
      c->data=f(c->data);
    return this;
  }

  inline Chain<TYPE>* each(void (*f)(const TYPE&)){
    Chain<TYPE>*c=this->rewind();
    while(c)
      f(c->data);
    return this;
  }

private:
  void operator=(const Chain<TYPE>* R){(void)R;}
};



#endif // MISC_H
