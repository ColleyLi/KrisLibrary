#ifndef GEOMETRY_GRID_SUBDIVISION_H
#define GEOMETRY_GRID_SUBDIVISION_H

#include <KrisLibrary/math/vector.h>
#include <KrisLibrary/utils/IntTuple.h>
#include <list>
#include <KrisLibrary/utils/stl_tr1.h>

namespace Geometry {

using namespace Math;

struct IndexHash
{
  IndexHash(size_t pow=257);
  size_t operator () (const std::vector<int>& x) const;
  size_t pow;
};

/** @ingroup Geometry
 * @brief A grid containing objects (referenced by void pointers)
 *
 * The map from a point x to an index is floor(x./h) where ./ indicates
 * element-wise division, and h is the resolution parameter defined on
 * construction (see Grid).
 */
class GridHash
{
public:
  typedef IntTuple Index;
  typedef void* Value;
  //called once per value in the query range, return false to stop enumerating
  typedef bool (*QueryCallback)(void* value);

  GridHash(int numDims,Real h=1);
  GridHash(const Vector& h);
  size_t GetBucketCount() const {  return buckets.bucket_count(); }
  void SetBucketCount(size_t n) {  buckets.rehash(n); }
  Vector GetResolution() const;
  ///Changes the resolution after construction -- need to have all buckets empty
  void SetResolution(const Vector& h);
  ///Changes the resolution after construction -- need to have all buckets empty
  void SetResolution(Real h);
  ///Sets the data at a given index
  void Set(const Index& i,void* data);
  ///Retrieves the data at a given index
  void* Get(const Index& i) const;
  ///Important: this method just removes the item from the hash, but does not delete its memory
  void* Erase(const Index& i); 
  ///Returns true if the hash contains the given index
  bool Contains(const Index& i);
  void Clear();
  void Enumerate(std::vector<Value>& items) const;

  //returns the index of the point
  void PointToIndex(const Vector& p,Index& i) const;
  //same, but with the local coordinates in the bucket [0,1]^n
  void PointToIndex(const Vector& p,Index& i,Vector& u) const;
  //returns the lower/upper corner of the bucket
  void IndexBucketBounds(const Index& i,Vector& bmin,Vector& bmax) const;

  //returns the min/max indices of all occupied cells
  void GetRange(Index& imin,Index& imax) const;
  //returns the min/max bound of all occupied cells
  void GetRange(Vector& bmin,Vector& bmax) const;

  //range imin to imax
  bool IndexQuery(const Index& imin,const Index& imax,QueryCallback f) const;
  //bounding box from bmin to bmax
  bool BoxQuery(const Vector& bmin,const Vector& bmax,QueryCallback f) const;
  //ball with center c, radius r
  bool BallQuery(const Vector& c,Real r,QueryCallback f) const;
  //segment from a to b
  bool SegmentQuery(const Vector& a,const Vector& b,QueryCallback f) const;

  //range imin to imax
  void IndexItems(const Index& imin,const Index& imax,std::vector<Value>& items) const;
  //bounding box from bmin to bmax
  void BoxItems(const Vector& bmin,const Vector& bmax,std::vector<Value>& items) const;
  //ball with center c, radius r
  void BallItems(const Vector& c,Real r,std::vector<Value>& items) const;
  //segment from a to b
  void SegmentItems(const Vector& a,const Vector& b,std::vector<Value>& items) const;

  Vector hinv;
  typedef UNORDERED_MAP_TEMPLATE<Index,Value,IndexHash> HashTable;
  HashTable buckets;
};


/** @ingroup Geometry
 * @brief A grid with a list of arbitrary objects (given by void pointers)
 */
class GridSubdivision
{
public:
  typedef IntTuple Index;
  typedef std::vector<void*> ObjectSet;
  //called once per object in the query range, return false to stop enumerating
  typedef bool (*QueryCallback)(void* obj);

  GridSubdivision(int numDims,Real h=1);
  GridSubdivision(const Vector& h);
  size_t GetBucketCount() const {  return buckets.bucket_count(); }
  void SetBucketCount(size_t n) {  buckets.rehash(n); }
  //this doesn't work -- hash power can't currently be changed.
  //void SetHashPower(size_t n) {  buckets.hash_function().pow=n; }
  void Insert(const Index& i,void* data);
  bool Erase(const Index& i,void* data);
  ObjectSet* GetObjectSet(const Index& i);
  const ObjectSet* GetObjectSet(const Index& i) const;
  void Clear();

  //returns the index of the point
  void PointToIndex(const Vector& p,Index& i) const;
  //same, but with the local coordinates in the bucket [0,1]^n
  void PointToIndex(const Vector& p,Index& i,Vector& u) const;
  //returns the lower/upper corner of the bucket
  void IndexBucketBounds(const Index& i,Vector& bmin,Vector& bmax) const;

  //returns the min/max indices of all occupied cells
  void GetRange(Index& imin,Index& imax) const;
  //returns the min/max bound of all occupied cells
  void GetRange(Vector& bmin,Vector& bmax) const;

  //range imin to imax
  bool IndexQuery(const Index& imin,const Index& imax,QueryCallback f) const;
  //bounding box from bmin to bmax
  bool BoxQuery(const Vector& bmin,const Vector& bmax,QueryCallback f) const;
  //ball with center c, radius r
  bool BallQuery(const Vector& c,Real r,QueryCallback f) const;
  //segment from a to b
  bool SegmentQuery(const Vector& a,const Vector& b,QueryCallback f) const;

  //range imin to imax
  void IndexItems(const Index& imin,const Index& imax,ObjectSet& objs) const;
  //bounding box from bmin to bmax
  void BoxItems(const Vector& bmin,const Vector& bmax,ObjectSet& objs) const;
  //ball with center c, radius r
  void BallItems(const Vector& c,Real r,ObjectSet& objs) const;
  //segment from a to b
  void SegmentItems(const Vector& a,const Vector& b,ObjectSet& objs) const;

  Vector hinv;
  typedef UNORDERED_MAP_TEMPLATE<Index,ObjectSet,IndexHash> HashTable;
  HashTable buckets;
};


} //namespace Geometry

#endif
