<memory>
shared_ptr<T>: ref counted smart pointer; multiple owners of the pointer; similar to poco shared_ptr
   the object pointed is not ref counter; shared_ptr<T> has the ref counter to record
   the number of reference. The deleter can be provided in the constructor

unique_ptr<T>: Stores a pointer to an owned object.one owner 
   The object is owned by no other unique_ptr. 
   The object is destroyed when the unique_ptr is destroyed.
   unique_ptr uniquely manages a resource. Each unique_ptr object stores a pointer to 
   the object that it owns or stores a null pointer. A resource can be owned by no more 
   than one unique_ptr object; when a unique_ptr object that owns a particular 
   resource is destroyed, the resource is freed. A unique_ptr object may be moved, 
   but not copied
      
weak_ptr<T>: not owner of the pointer, need transfer to shared_ptr<T> before using it.