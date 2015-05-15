#ifndef MISC_H
#define MISC_H

#include "./include/defines.h"

/**
 * Array class providing some common operations on tables.
 *
 * Class allowing user to store data in C table-like manner,
 * additionally providing certain JS-like fonctionality.
 * Template class getting stored data type name as template argument.
 */

template <class TYPE>
class Array {
private:
  TYPE * arr;
  int lengthC;
public:

  /**
   * @brief Array Array constructor
   * @param length length of newly created Array (can be increased later)
   */
  Array(int length) : arr(new TYPE [length]), lengthC(length) {}
  /**
   * @brief Array Array copy constructor
   * @param ref reference Array to be copied
   */
  Array(const Array<TYPE>& ref) : arr(new TYPE [ref.lengthC]), lengthC(ref.lengthC) {for (int i=0,l=ref.lengthC;i<l;i++)this->arr[i]=ref.arr[i];}
  ~Array(){delete [] this->arr;}

  /**
   * @brief length returns Array length
   * @return number of Array elements
   */
  inline int length() const {return this->lengthC;}

  /**
   * @brief getData returns data stored by Array
   * @param index index in Array of data to be returned
   * @return data data under requested index
   */
  TYPE getData(int index) const {return this->arr[index];}  //returns array field content

  /**
   * @brief setData replaces data stored by Array
   * @param d data to be inserted
   * @param index index of data to be replaced
   */
  void setData(TYPE d, int index){this->arr[index]=d;}  //writes data to array

  /**
   * @brief operator [] to access Array data in C table-like manner
   * @param index of data to be accessed
   * @return reference to data
   */
  TYPE& operator[](int index) const {return this->arr[index];}

  /**
   * @brief operator = for Array
   * @param R data to be assigned
   * @return reference
   */
  Array<TYPE>& operator=(const Array<TYPE>& R){
    delete [] this->arr;
    this->Array(R);
    return *this;
  }

  /**
   * @brief map replace content of all alements in array using provided function
   * @param f mapping function
   * @return reference to Array
   */
  Array<TYPE>& map(void (*f)(TYPE&)){//changes all values in array basing on value returned by passed function
    for (int i=0,l=this->lengthC;i<l;++i)
      f(this->arr[i]);
    return *this;
  }

  /**
   * @brief map replace content of all alements in array using provided function
   * @param f mapping function
   * @return reference to Array
   */
  Array<TYPE>& map(TYPE (*f)(const TYPE&)){//allows passed function to change all values in array
    for (int i=0,l=this->lengthC;i<l;++i)
      this->arr[i]=f(this->arr[i]);
    return *this;
  }

  /**
   * @brief each calls provided function on all elements of array
   * @param f mapping function
   * @return
   */
  Array<TYPE>& each(void (*f)(const TYPE&)){//calls passed function with all array elements as argument
    for (int i=0,l=this->lengthC;i<l;++i)
      f(this->arr[i]);
    return *this;
  }

  /**
   * @brief selected calls provided call function if acceptor function returns true
   * @param f call function
   * @param acceptor function
   * @return
   */
  Array<TYPE>& selected(void (*f)(const TYPE&), bool (*p)(const TYPE&)){//calls first passed function only on elements accepted by second one
    for (int i=0,l=this->lengthC;i<l;++i)
      if (p(this->arr[i]))
        f(this->arr[i]);
    return *this;
  }
};


/**
 * Chain class providing bi-directional-list like data organisation.
 *
 * Each elemen has 2 neighbours. data access to any field is linear.
 * Access to next/prev constant as well as adding/removing after/before current element.
 * Due to its nature each element can be considered autonomous list thus each element
 * popped from list using pop method becomes new list (in constant time/0 alloc)
 * Template class gets stored data type as template param.
 */
template <class TYPE>
class Chain {
private:
  Chain * nextC;
  Chain * prevC;
public:

  TYPE data; /**< data stored in each element */

  /**
   * @brief Chain Chain constructor. Reqires data to be written into first
   * @param data data to be written as first element
   */
  Chain(TYPE data) : nextC(nullptr), prevC(nullptr), data(data) {}
  ~Chain(){this->pop();}

  /**
   * @brief next
   * @return next chain elem
   */
  Chain * next(){return this->nextC;}

  /**
   * @brief prev
   * @return prev chain elem
   */
  Chain * prev(){return this->prevC;}

  /**
   * @brief rewind
   * @return first chain elem
   */
  Chain * rewind(){Chain<TYPE>*c=this;while(c->prev())c=c->prev();return c;}  //returns first element

  /**
   * @brief rewindForward
   * @return last chain elem
   */
  Chain * rewindForward(){Chain<TYPE>*c=this;while(c->next())c=c->next();return c;}  //returns last element

  /**
   * @brief insertAfter inserts given subchain after current element
   * @param c subchain
   */
  inline void insertAfter(Chain * c){  //inserts given chain after current element
    Chain*t=this->nextC;
    this->nextC=c;
    c->prevC=this;
    c=c->rewindForward();
    c->nextC=t;
    if (t)
      t->prevC=c;
  }

  /**
   * @brief insertBefore inserts given subchain before current elmenet
   * @param c subchain
   */
  inline void insertBefore(Chain * c){  //inserts given chain before current element
    Chain*t=this->prevC;
    this->prevC=c;
    c->nextC=this;
    c=c->rewind();
    c->prevC=t;
    if (t)
      t->nextC=c;
  }

  /**
   * @brief insertAfter inserts given data after current element
   * @param data data
   */
  inline void insertAfter(TYPE data){this->insertAfter(new Chain(data));}  //inserts given data after current element

  /**
   * @brief insertBefore inserts given data before current elmenet
   * @param data data
   */
  inline void insertBefore(TYPE data){this->insertBefore(new Chain(data));}  //inserts given data before current element

  /**
   * @brief pop removes element from chain, creating new chain
   */
  inline void pop(){  //pulls element out of chain (doesn't delete it)
    if(this->nextC)
      this->nextC->prevC=this->prevC;
    if(this->prevC)
      this->prevC->nextC=this->nextC;
  }

  /**
   * @brief map replace content of all alements in array using provided function
   * @param f mapping function
   * @return reference to Array
   */
  inline Chain<TYPE>* map(void (*f)(TYPE&)){//changes all values in array basing on value returned by passed function
    Chain<TYPE>*c=this->rewind();
    while(c)
      f(c->data);
    return this;
  }

  /**
   * @brief map replace content of all alements in array using provided function
   * @param f mapping function
   * @return reference to Array
   */
  inline Chain<TYPE>* map(TYPE (*f)(const TYPE&)){//allows passed function to change all values in array
    Chain<TYPE>*c=this->rewind();
    while(c)
      c->data=f(c->data);
    return this;
  }

  /**
   * @brief each calls provided function on all elements of array
   * @param f mapping function
   * @return
   */
  inline Chain<TYPE>* each(void (*f)(const TYPE&)){//calls passed function with all array elements as argument
    Chain<TYPE>*c=this->rewind();
    while(c)
      f(c->data);
    return this;
  }

  /**
   * @brief selected calls provided call function if acceptor function returns true
   * @param f call function
   * @param acceptor function
   * @return
   */
  inline Chain<TYPE>* selected(void (*f)(const TYPE&), bool (*p)(const TYPE&)){//calls first passed function only on elements accepted by second one
    Chain<TYPE>*c=this->rewind();
    while(c)
      if (p(c->data))
        f(c->data);
    return this;
  }

private:
  void operator=(const Chain<TYPE>* R){(void)R;}  //locked
};



#endif // MISC_H
