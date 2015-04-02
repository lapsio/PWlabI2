#ifndef GC_H
#define GC_H

class GCRefLink;
class GCRef;



class GCRefLink {
  friend class GCRef;
private:
  GCRef * ref;
  GCRefLink(GCRef * ref);

public:
  ~GCRefLink();
};

class GCRef {
  friend class GCRefLink;
private:
  int refCount;
  bool lock;

  inline void unlink();

public:
  GCRefLink * link();
  void unlock();
  int refUsersCount() const;

protected:
  GCRef();
  virtual ~GCRef(){}
};

#endif // GC_H
