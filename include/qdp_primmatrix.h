// -*- C++ -*-
// $Id: qdp_primmatrix.h,v 1.20 2004-07-06 01:54:28 edwards Exp $

/*! \file
 * \brief Primitive Matrix
 */

QDP_BEGIN_NAMESPACE(QDP);

//-------------------------------------------------------------------------------------
/*! \addtogroup primmatrix Matrix primitive
 * \ingroup fiber
 *
 * Primitive type that transforms like a matrix
 *
 * @{
 */


//! Primitive Matrix class
/*!
 * All Matrix classes inherit this class
 * NOTE: For efficiency, there can be no virtual methods, so the data
 * portion is a part of the generic class, hence it is called a domain
 * and not a category
 */
template <class T, int N, template<class,int> class C> class PMatrix
{
public:
  PMatrix() {}
  ~PMatrix() {}

  typedef C<T,N>  CC;

  //! PMatrix = PScalar
  /*! Fill with primitive scalar */
  template<class T1>
  inline
  CC& assign(const PScalar<T1>& rhs)
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  if (i == j)
	    elem(i,j) = rhs.elem();
	  else
	    zero_rep(elem(i,j));

      return static_cast<CC&>(*this);
    }

  //! PMatrix = PMatrix
  /*! Set equal to another PMatrix */
  template<class T1>
  inline
  CC& assign(const C<T1,N>& rhs) 
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  elem(i,j) = rhs.elem(i,j);

      return static_cast<CC&>(*this);
    }

  //! PMatrix += PMatrix
  template<class T1>
  inline
  CC& operator+=(const C<T1,N>& rhs) 
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  elem(i,j) += rhs.elem(i,j);

      return static_cast<CC&>(*this);
    }

  //! PMatrix -= PMatrix
  template<class T1>
  inline
  CC& operator-=(const C<T1,N>& rhs) 
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  elem(i,j) -= rhs.elem(i,j);

      return static_cast<CC&>(*this);
    }

  //! PMatrix += PScalar
  template<class T1>
  inline
  CC& operator+=(const PScalar<T1>& rhs) 
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  elem(i,j) += rhs.elem();

      return static_cast<CC&>(*this);
    }

  //! PMatrix -= PScalar
  template<class T1>
  inline
  CC& operator-=(const PScalar<T1>& rhs) 
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  elem(i,j) -= rhs.elem();

      return static_cast<CC&>(*this);
    }

  //! PMatrix *= PScalar
  template<class T1>
  inline
  CC& operator*=(const PScalar<T1>& rhs) 
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  elem(i,j) *= rhs.elem();

      return static_cast<CC&>(*this);
    }

  //! PMatrix /= PScalar
  template<class T1>
  inline
  CC& operator/=(const PScalar<T1>& rhs) 
    {
      for(int i=0; i < N; ++i)
	for(int j=0; j < N; ++j)
	  elem(i,j) /= rhs.elem();

      return static_cast<CC&>(*this);
    }

#if 0
  // NOTE: intentially avoid defining a copy constructor - let the compiler
  // generate one via the bit copy mechanism. This effectively achieves
  // the first form of the if below (QDP_USE_ARRAY_INITIALIZER) without having
  // to use that syntax which is not strictly legal in C++.

  //! Deep copy here
#if defined(QDP_USE_ARRAY_INITIALIZER)
  /*! This is an array initializer form - may not be strictly legal */
  PMatrix(const PMatrix& a) : F(a.F) {}
#else
  /*! This is a copy form - legal but not necessarily efficient */
  PMatrix(const PMatrix& a)
    {
      for(int i=0; i < N*N; ++i)
	F[i] = a.F[i];
    }
#endif
#endif

public:
  T& elem(int i, int j) {return F[j+N*i];}
  const T& elem(int i, int j) const {return F[j+N*i];}

private:
  T F[N*N];
};


//! Text input
template<class T, int N, template<class,int> class C>  
inline
TextReader& operator>>(TextReader& txt, PMatrix<T,N,C>& d)
{
  for(int j=0; j < N; ++j)
    for(int i=0; i < N; ++i)
      txt >> d.elem(i,j);

  return txt;
}

//! Text output
template<class T, int N, template<class,int> class C>  
inline
TextWriter& operator<<(TextWriter& txt, const PMatrix<T,N,C>& d)
{
  for(int j=0; j < N; ++j)
    for(int i=0; i < N; ++i)
      txt << d.elem(i,j);

  return txt;
}


//! Nml output
template<class T, int N, template<class,int> class C>  
inline
NmlWriter& operator<<(NmlWriter& nml, const PMatrix<T,N,C>& d)
{
  nml.get() << "  [MATRIX]\n";
  for(int j=0; j < N-1; ++j)
    for(int i=0; i < N; ++i)
    {
      nml.get() << "\tRow = " << i << ", Column = " << j << " = ";
      nml << d.elem(i,j);
      nml.get() << ",\n";
    }
    
  int j=N-1;
  for(int i=0; i < N-1; ++i)
  {
    nml.get() << "\tRow = " << i << ", Column = " << j << " = ";
    nml << d.elem(i,j);
    nml.get() << ",\n";
  }
    
  int i = j;
  nml.get() << "\tRow = " << i << ", Column = " << j << " = ";
  nml << d.elem(i,j);

  return nml;
}


//! XML output
template<class T, int N, template<class,int> class C>  
inline
XMLWriter& operator<<(XMLWriter& xml, const PMatrix<T,N,C>& d)
{
  xml.openTag("Matrix");

  XMLWriterAPI::AttributeList alist;

  for(int i=0; i < N; ++i)
  {
    for(int j=0; j < N; ++j)
    {
      alist.clear();
      alist.push_back(XMLWriterAPI::Attribute("row", i));
      alist.push_back(XMLWriterAPI::Attribute("col", j));

      xml.openTag("elem", alist);
      xml << d.elem(i,j);
      xml.closeTag();
    }
  }

  xml.closeTag(); // Matrix
  return xml;
}

/*! @} */  // end of group primmatrix

//-----------------------------------------------------------------------------
// Traits classes 
//-----------------------------------------------------------------------------

// Underlying word type
template<class T1, int N, template<class,int> class C>
struct WordType<PMatrix<T1,N,C> > 
{
  typedef typename WordType<T1>::Type_t  Type_t;
};

// Internally used scalars
template<class T, int N, template<class,int> class C>
struct InternalScalar<PMatrix<T,N,C> > {
  typedef PScalar<typename InternalScalar<T>::Type_t>  Type_t;
};

// Makes a primitive scalar leaving grid alone
template<class T, int N, template<class,int> class C>
struct PrimitiveScalar<PMatrix<T,N,C> > {
  typedef PScalar<typename PrimitiveScalar<T>::Type_t>  Type_t;
};

// Makes a lattice scalar leaving primitive indices alone
template<class T, int N, template<class,int> class C>
struct LatticeScalar<PMatrix<T,N,C> > {
  typedef C<typename LatticeScalar<T>::Type_t, N>  Type_t;
};


//-----------------------------------------------------------------------------
// Traits classes to support return types
//-----------------------------------------------------------------------------


// Default unary(PMatrix) -> PMatrix
template<class T, int N, template<class,int> class C, class Op>
struct UnaryReturn<PMatrix<T,N,C>, Op> {
  typedef C<typename UnaryReturn<T, Op>::Type_t, N>  Type_t;
};

// Default binary(PScalar,PMatrix) -> PMatrix
template<class T1, class T2, int N, template<class,int> class C, class Op>
struct BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, Op> {
  typedef C<typename BinaryReturn<T1, T2, Op>::Type_t, N>  Type_t;
};

// Default binary(PMatrix,PMatrix) -> PMatrix
template<class T1, class T2, int N, template<class,int> class C, class Op>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, Op> {
  typedef C<typename BinaryReturn<T1, T2, Op>::Type_t, N>  Type_t;
};

// Default binary(PMatrix,PScalar) -> PMatrix
template<class T1, int N, class T2, template<class,int> class C, class Op>
struct BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, Op> {
  typedef C<typename BinaryReturn<T1, T2, Op>::Type_t, N>  Type_t;
};



#if 0
template<class T1, class T2>
struct UnaryReturn<PScalar<T2>, OpCast<T1> > {
  typedef PScalar<typename UnaryReturn<T, OpCast>::Type_t>  Type_t;
//  typedef T1 Type_t;
};
#endif


// Assignment is different
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAssign > {
  typedef C<T1,N> &Type_t;
};
 
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAddAssign > {
  typedef C<T1,N> &Type_t;
};
 
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpSubtractAssign > {
  typedef C<T1,N> &Type_t;
};
 
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpAssign > {
  typedef C<T1,N> &Type_t;
};
 
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpAddAssign > {
  typedef C<T1,N> &Type_t;
};
 
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpSubtractAssign > {
  typedef C<T1,N> &Type_t;
};
 
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpMultiplyAssign > {
  typedef C<T1,N> &Type_t;
};
 
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpDivideAssign > {
  typedef C<T1,N> &Type_t;
};
 


//-----------------------------------------------------------------------------
// Operators
//-----------------------------------------------------------------------------
/*! \addtogroup primmatrix */
/*! @{ */

// Primitive Matrices

//  + PMatrix
template<class T1, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, OpUnaryPlus>::Type_t
operator+(const PMatrix<T1,N,C>& l)
{
  typename UnaryReturn<PMatrix<T1,N,C>, OpUnaryPlus>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = +l.elem(i,j);

  return d;
}


//  - PMatrix
template<class T1, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, OpUnaryMinus>::Type_t
operator-(const PMatrix<T1,N,C>& l)
{
  typename UnaryReturn<PMatrix<T1,N,C>, OpUnaryMinus>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = -l.elem(i,j);

  return d;
}


// PMatrix - PMatrix
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAdd>::Type_t
operator+(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAdd>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = l.elem(i,j) + r.elem(i,j);

  return d;
}


// PMatrix - PMatrix
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpSubtract>::Type_t
operator-(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpSubtract>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = l.elem(i,j) - r.elem(i,j);

  return d;
}


// PMatrix * PScalar
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpMultiply>::Type_t
operator*(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = l.elem(i,j) * r.elem();
  return d;
}

// Optimized  adj(PMatrix)*PScalar
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpAdjMultiply>::Type_t
adjMultiply(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpAdjMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = adjMultiply(l.elem(j,i), r.elem());
  return d;
}

// Optimized  PMatrix*adj(PScalar)
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpMultiplyAdj>::Type_t
multiplyAdj(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpMultiplyAdj>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = multiplyAdj(l.elem(i,j), r.elem());
  return d;
}

// Optimized  adj(PMatrix)*adj(PScalar)
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpAdjMultiplyAdj>::Type_t
adjMultiplyAdj(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpAdjMultiplyAdj>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = adjMultiplyAdj(l.elem(j,i), r.elem());
  return d;
}



// PScalar * PMatrix
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpMultiply>::Type_t
operator*(const PScalar<T1>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = l.elem() * r.elem(i,j);
  return d;
}

// Optimized adj(PScalar) * PMatrix
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpAdjMultiply>::Type_t
adjMultiply(const PScalar<T1>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpAdjMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = adjMultiply(l.elem(), r.elem(i,j));
  return d;
}

// Optimized PScalar * adj(PMatrix)
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpMultiplyAdj>::Type_t
multiplyAdj(const PScalar<T1>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpMultiplyAdj>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = multiplyAdj(l.elem(), r.elem(j,i));
  return d;
}

// Optimized adj(PScalar) * adj(PMatrix)
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpAdjMultiplyAdj>::Type_t
adjMultiplyAdj(const PScalar<T1>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, OpAdjMultiplyAdj>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = adjMultiplyAdj(l.elem(), r.elem(j,i));
  return d;
}


// PMatrix * PMatrix
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpMultiply>::Type_t
operator*(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
    {
      d.elem(i,j) = l.elem(i,0) * r.elem(0,j);
      for(int k=1; k < N; ++k)
	d.elem(i,j) += l.elem(i,k) * r.elem(k,j);
    }

  return d;
}

// Optimized  adj(PMatrix)*PMatrix
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAdjMultiply>::Type_t
adjMultiply(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAdjMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
    {
      d.elem(i,j) = adjMultiply(l.elem(0,i), r.elem(0,j));
      for(int k=1; k < N; ++k)
	d.elem(i,j) += adjMultiply(l.elem(k,i), r.elem(k,j));
    }

  return d;
}

// Optimized  PMatrix*adj(PMatrix)
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpMultiplyAdj>::Type_t
multiplyAdj(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpMultiplyAdj>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
    {
      d.elem(i,j) = multiplyAdj(l.elem(i,0), r.elem(j,0));
      for(int k=1; k < N; ++k)
	d.elem(i,j) += multiplyAdj(l.elem(i,k), r.elem(j,k));
    }

  return d;
}

// Optimized  adj(PMatrix)*adj(PMatrix)
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAdjMultiplyAdj>::Type_t
adjMultiplyAdj(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, OpAdjMultiplyAdj>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
    {
      d.elem(i,j) = adjMultiplyAdj(l.elem(0,i), r.elem(j,0));
      for(int k=1; k < N; ++k)
	d.elem(i,j) += adjMultiplyAdj(l.elem(k,i), r.elem(j,k));
    }

  return d;
}


// PMatrix / PScalar
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpDivide>::Type_t
operator/(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, OpDivide>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = l.elem(i,j) / r.elem();
  return d;
}



//-----------------------------------------------------------------------------
// Functions

// Adjoint
template<class T1, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnAdjoint>::Type_t
adj(const PMatrix<T1,N,C>& l)
{
  typename UnaryReturn<PMatrix<T1,N,C>, FnAdjoint>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = adj(l.elem(j,i));

  return d;
}


// Conjugate
template<class T1, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnConjugate>::Type_t
conj(const PMatrix<T1,N,C>& l)
{
  typename UnaryReturn<PMatrix<T1,N,C>, FnConjugate>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = conj(l.elem(i,j));

  return d;
}


// Transpose
template<class T1, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnTranspose>::Type_t
transpose(const PMatrix<T1,N,C>& l)
{
  typename UnaryReturn<PMatrix<T1,N,C>, FnTranspose>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = transpose(l.elem(j,i));

  return d;
}


// TRACE
// PScalar = Trace(PMatrix)
template<class T, int N, template<class,int> class C>
struct UnaryReturn<PMatrix<T,N,C>, FnTrace > {
  typedef PScalar<typename UnaryReturn<T, FnTrace>::Type_t>  Type_t;
};

template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnTrace>::Type_t
trace(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnTrace>::Type_t  d;

  d.elem() = trace(s1.elem(0,0));
  for(int i=1; i < N; ++i)
    d.elem() += trace(s1.elem(i,i));

  return d;
}


// PScalar = Re(Trace(PMatrix))
template<class T, int N, template<class,int> class C>
struct UnaryReturn<PMatrix<T,N,C>, FnRealTrace > {
  typedef PScalar<typename UnaryReturn<T, FnRealTrace>::Type_t>  Type_t;
};

template<class T1, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnRealTrace>::Type_t
realTrace(const PMatrix<T1,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T1,N,C>, FnRealTrace>::Type_t  d;

  d.elem() = realTrace(s1.elem(0,0));
  for(int i=1; i < N; ++i)
    d.elem() += realTrace(s1.elem(i,i));

  return d;
}


//! PScalar = Im(Trace(PMatrix))
template<class T, int N, template<class,int> class C>
struct UnaryReturn<PMatrix<T,N,C>, FnImagTrace > {
  typedef PScalar<typename UnaryReturn<T, FnImagTrace>::Type_t>  Type_t;
};

template<class T1, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnImagTrace>::Type_t
imagTrace(const PMatrix<T1,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T1,N,C>, FnImagTrace>::Type_t  d;

  d.elem() = imagTrace(s1.elem(0,0));
  for(int i=1; i < N; ++i)
    d.elem() += imagTrace(s1.elem(i,i));

  return d;
}


//! PMatrix = traceColor(PMatrix)   [this is an identity in general]
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnTraceColor>::Type_t
traceColor(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnTraceColor>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = traceColor(s1.elem(i,j));

  return d;
}


//! PMatrix = traceSpin(PMatrix)   [this is an identity in general]
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnTraceSpin>::Type_t
traceSpin(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnTraceSpin>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = traceSpin(s1.elem(i,j));

  return d;
}


// PScalar = traceMultiply(PMatrix,PMatrix)
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnTraceMultiply> {
  typedef PScalar<typename BinaryReturn<T1, T2, FnTraceMultiply>::Type_t>  Type_t;
};

template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnTraceMultiply>::Type_t
traceMultiply(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnTraceMultiply>::Type_t  d;

  d.elem() = traceMultiply(l.elem(0,0), r.elem(0,0));
  for(int k=1; k < N; ++k)
    d.elem() += traceMultiply(l.elem(0,k), r.elem(k,0));

  for(int j=1; j < N; ++j)
    for(int k=0; k < N; ++k)
      d.elem() += traceMultiply(l.elem(j,k), r.elem(k,j));

  return d;
}

// PScalar = traceMultiply(PMatrix,PScalar)
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, FnTraceMultiply> {
  typedef PScalar<typename BinaryReturn<T1, T2, FnTraceMultiply>::Type_t>  Type_t;
};

template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, FnTraceMultiply>::Type_t
traceMultiply(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, FnTraceMultiply>::Type_t  d;

  d.elem() = traceMultiply(l.elem(0,0), r.elem());
  for(int k=1; k < N; ++k)
    d.elem() += traceMultiply(l.elem(k,k), r.elem());

  return d;
}

// PScalar = traceMultiply(PScalar,PMatrix)
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, FnTraceMultiply> {
  typedef PScalar<typename BinaryReturn<T1, T2, FnTraceMultiply>::Type_t>  Type_t;
};

template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, FnTraceMultiply>::Type_t
traceMultiply(const PScalar<T1>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, FnTraceMultiply>::Type_t  d;

  d.elem() = traceMultiply(l.elem(), r.elem(0,0));
  for(int k=1; k < N; ++k)
    d.elem() += traceMultiply(l.elem(), r.elem(k,k));

  return d;
}



//! PMatrix = traceColorMultiply(PMatrix,PMatrix)   [the trace is an identity in general]
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnTraceColorMultiply>::Type_t
traceColorMultiply(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnTraceColorMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
    {
      d.elem(i,j) = traceColorMultiply(l.elem(i,0), r.elem(0,j));
      for(int k=1; k < N; ++k)
	d.elem(i,j) += traceColorMultiply(l.elem(i,k), r.elem(k,j));
    }

  return d;
}

// PScalar = traceColorMultiply(PMatrix,PScalar)   [the trace is an identity in general]
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, FnTraceColorMultiply>::Type_t
traceColorMultiply(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, FnTraceColorMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = traceColorMultiply(l.elem(i,j), r.elem());

  return d;
}

// PScalar = traceColorMultiply(PScalar,PMatrix)   [the trace is an identity in general]
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, FnTraceColorMultiply>::Type_t
traceColorMultiply(const PScalar<T1>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, FnTraceColorMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = traceColorMultiply(l.elem(), r.elem(i,j));

  return d;
}


//! PMatrix = traceSpinMultiply(PMatrix,PMatrix)   [the trace is an identity in general]
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnTraceSpinMultiply>::Type_t
traceSpinMultiply(const PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnTraceSpinMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
    {
      d.elem(i,j) = traceSpinMultiply(l.elem(i,0), r.elem(0,j));
      for(int k=1; k < N; ++k)
	d.elem(i,j) += traceSpinMultiply(l.elem(i,k), r.elem(k,j));
    }

  return d;
}

// PScalar = traceSpinMultiply(PMatrix,PScalar)   [the trace is an identity in general]
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, FnTraceSpinMultiply>::Type_t
traceSpinMultiply(const PMatrix<T1,N,C>& l, const PScalar<T2>& r)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PScalar<T2>, FnTraceSpinMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = traceSpinMultiply(l.elem(i,j), r.elem());

  return d;
}

// PScalar = traceSpinMultiply(PScalar,PMatrix)   [the trace is an identity in general]
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, FnTraceSpinMultiply>::Type_t
traceSpinMultiply(const PScalar<T1>& l, const PMatrix<T2,N,C>& r)
{
  typename BinaryReturn<PScalar<T1>, PMatrix<T2,N,C>, FnTraceSpinMultiply>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = traceSpinMultiply(l.elem(), r.elem(i,j));

  return d;
}


//! PMatrix = Re(PMatrix)
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnReal>::Type_t
real(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnReal>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = real(s1.elem(i,j));

  return d;
}


//! PMatrix = Im(PMatrix)
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnImag>::Type_t
imag(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnImag>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = imag(s1.elem(i,j));

  return d;
}


//! PMatrix<T> = (PMatrix<T> , PMatrix<T>)
template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnCmplx>::Type_t
cmplx(const PMatrix<T1,N,C>& s1, const PMatrix<T2,N,C>& s2)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnCmplx>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = cmplx(s1.elem(i,j), s2.elem(i,j));

  return d;
}




// Functions
//! PMatrix = i * PMatrix
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnTimesI>::Type_t
timesI(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnTimesI>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = timesI(s1.elem(i,j));

  return d;
}

//! PMatrix = -i * PMatrix
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnTimesMinusI>::Type_t
timesMinusI(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnTimesMinusI>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = timesMinusI(s1.elem(i,j));

  return d;
}

//! dest [some type] = source [some type]
/*! Portable (internal) way of returning a single site */
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnGetSite>::Type_t
getSite(const PMatrix<T,N,C>& s1, int innersite)
{ 
  typename UnaryReturn<PMatrix<T,N,C>, FnGetSite>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = getSite(s1.elem(i,j), innersite);

  return d;
}

//! Extract color vector components 
/*! Generically, this is an identity operation. Defined differently under color */
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnPeekColorVector>::Type_t
peekColor(const PMatrix<T,N,C>& l, int row)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnPeekColorVector>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = peekColor(l.elem(i,j),row);
  return d;
}

//! Extract color matrix components 
/*! Generically, this is an identity operation. Defined differently under color */
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnPeekColorMatrix>::Type_t
peekColor(const PMatrix<T,N,C>& l, int row, int col)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnPeekColorMatrix>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = peekColor(l.elem(i,j),row,col);
  return d;
}

//! Extract spin vector components 
/*! Generically, this is an identity operation. Defined differently under spin */
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnPeekSpinVector>::Type_t
peekSpin(const PMatrix<T,N,C>& l, int row)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnPeekSpinVector>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = peekSpin(l.elem(i,j),row);
  return d;
}

//! Extract spin matrix components 
/*! Generically, this is an identity operation. Defined differently under spin */
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnPeekSpinMatrix>::Type_t
peekSpin(const PMatrix<T,N,C>& l, int row, int col)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnPeekSpinMatrix>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = peekSpin(l.elem(i,j),row,col);
  return d;
}

//! Extract domain-wall vector components 
/*! Generically, this is an identity operation. Defined differently under domain-wall index */
template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnPeekDWVector>::Type_t
peekDW(const PMatrix<T,N,C>& l, int row)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnPeekDWVector>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = peekDW(l.elem(i,j),row);
  return d;
}


//! Insert color vector components 
/*! Generically, this is an identity operation. Defined differently under color */
template<class T1, class T2, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnPokeColorMatrix>::Type_t&
pokeColor(PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r, int row)
{
  typedef typename UnaryReturn<PMatrix<T1,N,C>, FnPokeColorMatrix>::Type_t  Return_t;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      pokeColor(l.elem(i,j),r.elem(i,j),row);
  return static_cast<Return_t&>(l);
}

//! Insert color matrix components 
/*! Generically, this is an identity operation. Defined differently under color */
template<class T1, class T2, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnPokeColorMatrix>::Type_t&
pokeColor(PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r, int row, int col)
{
  typedef typename UnaryReturn<PMatrix<T1,N,C>, FnPokeColorMatrix>::Type_t  Return_t;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      pokeColor(l.elem(i,j),r.elem(i,j),row,col);
  return static_cast<Return_t&>(l);
}

//! Insert spin vector components 
/*! Generically, this is an identity operation. Defined differently under spin */
template<class T1, class T2, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnPokeSpinMatrix>::Type_t&
pokeSpin(PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r, int row)
{
  typedef typename UnaryReturn<PMatrix<T1,N,C>, FnPokeSpinMatrix>::Type_t  Return_t;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      pokeSpin(l.elem(i,j),r.elem(i,j),row);
  return static_cast<Return_t&>(l);
}

//! Insert spin matrix components 
/*! Generically, this is an identity operation. Defined differently under spin */
template<class T1, class T2, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnPokeSpinMatrix>::Type_t&
pokeSpin(PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r, int row, int col)
{
  typedef typename UnaryReturn<PMatrix<T1,N,C>, FnPokeSpinMatrix>::Type_t  Return_t;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      pokeSpin(l.elem(i,j),r.elem(i,j),row,col);
  return static_cast<Return_t&>(l);
}

//! Insert domain-wall vector components 
/*! Generically, this is an identity operation. Defined differently under domain-wall */
template<class T1, class T2, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T1,N,C>, FnPokeDWMatrix>::Type_t&
pokeDW(PMatrix<T1,N,C>& l, const PMatrix<T2,N,C>& r, int row)
{
  typedef typename UnaryReturn<PMatrix<T1,N,C>, FnPokeDWMatrix>::Type_t  Return_t;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      pokeDW(l.elem(i,j),r.elem(i,j),row);
  return static_cast<Return_t&>(l);
}



//! dest = 0
template<class T, int N, template<class,int> class C> 
inline void 
zero_rep(PMatrix<T,N,C>& dest) 
{
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      zero_rep(dest.elem(i,j));
}


//! dest = (mask) ? s1 : dest
template<class T, class T1, int N, template<class,int> class C> 
inline void 
copymask(PMatrix<T,N,C>& d, const PScalar<T1>& mask, const PMatrix<T,N,C>& s1) 
{
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      copymask(d.elem(i,j),mask.elem(),s1.elem(i,j));
}


//! dest [some type] = source [some type]
template<class T, class T1, int N, template<class,int> class C>
inline void 
copy_site(PMatrix<T,N,C>& d, int isite, const PMatrix<T1,N,C>& s1)
{
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      copy_site(d.elem(i,j), isite, s1.elem(i,j));
}

//! dest [some type] = source [some type]
template<class T, class T1, int N, template<class,int> class C>
inline void 
copy_site(PMatrix<T,N,C>& d, int isite, const PScalar<T1>& s1)
{
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      copy_site(d.elem(i,j), isite, s1.elem());
}


//! gather several inner sites together
template<class T, class T1, int N, template<class,int> class C>
inline void 
gather_sites(PMatrix<T,N,C>& d, 
	     const PMatrix<T1,N,C>& s0, int i0, 
	     const PMatrix<T1,N,C>& s1, int i1,
	     const PMatrix<T1,N,C>& s2, int i2,
	     const PMatrix<T1,N,C>& s3, int i3)
{
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      gather_sites(d.elem(i,j), 
		   s0.elem(i,j), i0, 
		   s1.elem(i,j), i1, 
		   s2.elem(i,j), i2, 
		   s3.elem(i,j), i3);
}


//! dest  = random  
template<class T, int N, template<class,int> class C, class T1, class T2>
inline void
fill_random(PMatrix<T,N,C>& d, T1& seed, T2& skewed_seed, const T1& seed_mult)
{
  // The skewed_seed is the starting seed to use
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      fill_random(d.elem(i,j), seed, skewed_seed, seed_mult);
}

//! dest  = gaussian
template<class T, int N, template<class,int> class C>
inline void
fill_gaussian(PMatrix<T,N,C>& d, PMatrix<T,N,C>& r1, PMatrix<T,N,C>& r2)
{
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      fill_gaussian(d.elem(i,j), r1.elem(i,j), r2.elem(i,j));
}



#if 0
// Global sum over site indices only
template<class T, int N, template<class,int> class C>
struct UnaryReturn<PMatrix<T,N,C>, FnSum > {
  typedef C<typename UnaryReturn<T, FnSum>::Type_t, N>  Type_t;
};

template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnSum>::Type_t
sum(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnSum>::Type_t  d;

  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = sum(s1.elem(i,j));

  return d;
}
#endif


// InnerProduct (norm-seq) global sum = sum(tr(adj(s1)*s1))
template<class T, int N, template<class,int> class C>
struct UnaryReturn<PMatrix<T,N,C>, FnNorm2 > {
  typedef PScalar<typename UnaryReturn<T, FnNorm2>::Type_t>  Type_t;
};

template<class T, int N, template<class,int> class C>
struct UnaryReturn<PMatrix<T,N,C>, FnLocalNorm2 > {
  typedef PScalar<typename UnaryReturn<T, FnLocalNorm2>::Type_t>  Type_t;
};

template<class T, int N, template<class,int> class C>
inline typename UnaryReturn<PMatrix<T,N,C>, FnLocalNorm2>::Type_t
localNorm2(const PMatrix<T,N,C>& s1)
{
  typename UnaryReturn<PMatrix<T,N,C>, FnLocalNorm2>::Type_t  d;

  d.elem() = localNorm2(s1.elem(0,0));
  for(int j=1; j < N; ++j)
    d.elem() += localNorm2(s1.elem(0,j));

  for(int i=1; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem() += localNorm2(s1.elem(i,j));

  return d;
}


//! PScalar<T> = InnerProduct(adj(PMatrix<T1>)*PMatrix<T1>)
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnInnerProduct > {
  typedef PScalar<typename BinaryReturn<T1, T2, FnInnerProduct>::Type_t>  Type_t;
};

template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnLocalInnerProduct > {
  typedef PScalar<typename BinaryReturn<T1, T2, FnLocalInnerProduct>::Type_t>  Type_t;
};

template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnLocalInnerProduct>::Type_t
localInnerProduct(const PMatrix<T1,N,C>& s1, const PMatrix<T2,N,C>& s2)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnLocalInnerProduct>::Type_t  d;

  d.elem() = localInnerProduct(s1.elem(0,0), s2.elem(0,0));
  for(int k=1; k < N; ++k)
    d.elem() += localInnerProduct(s1.elem(k,0), s2.elem(k,0));

  for(int j=1; j < N; ++j)
    for(int k=0; k < N; ++k)
      d.elem() += localInnerProduct(s1.elem(k,j), s2.elem(k,j));

  return d;
}


//! PScalar<T> = InnerProductReal(adj(PMatrix<T1>)*PMatrix<T1>)
/*!
 * return  realpart of InnerProduct(adj(s1)*s2)
 */
template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnInnerProductReal > {
  typedef PScalar<typename BinaryReturn<T1, T2, FnInnerProductReal>::Type_t>  Type_t;
};

template<class T1, class T2, int N, template<class,int> class C>
struct BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnLocalInnerProductReal > {
  typedef PScalar<typename BinaryReturn<T1, T2, FnLocalInnerProductReal>::Type_t>  Type_t;
};

template<class T1, class T2, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnLocalInnerProductReal>::Type_t
localInnerProductReal(const PMatrix<T1,N,C>& s1, const PMatrix<T2,N,C>& s2)
{
  typename BinaryReturn<PMatrix<T1,N,C>, PMatrix<T2,N,C>, FnLocalInnerProductReal>::Type_t  d;

  d.elem() = localInnerProductReal(s1.elem(0,0), s2.elem(0,0));
  for(int k=1; k < N; ++k)
    d.elem() += localInnerProductReal(s1.elem(k,0), s2.elem(k,0));

  for(int j=1; j < N; ++j)
    for(int k=0; k < N; ++k)
      d.elem() += localInnerProductReal(s1.elem(k,j), s2.elem(k,j));

  return d;
}


//! PMatrix<T> = where(PScalar, PMatrix, PMatrix)
/*!
 * Where is the ? operation
 * returns  (a) ? b : c;
 */
template<class T1, class T2, class T3, int N, template<class,int> class C>
inline typename BinaryReturn<PMatrix<T2,N,C>, PMatrix<T3,N,C>, FnWhere>::Type_t
where(const PScalar<T1>& a, const PMatrix<T2,N,C>& b, const PMatrix<T3,N,C>& c)
{
  typename BinaryReturn<PMatrix<T2,N,C>, PMatrix<T3,N,C>, FnWhere>::Type_t  d;

  // Not optimal - want to have where outside assignment
  for(int i=0; i < N; ++i)
    for(int j=0; j < N; ++j)
      d.elem(i,j) = where(a.elem(), b.elem(i,j), c.elem(i,j));

  return d;
}

/*! @} */  // end of group primmatrix

QDP_END_NAMESPACE();

