/*************************************************************************

Copyright Rice University, 2004.
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, provided that the above copyright notice(s) and this
permission notice appear in all copies of the Software and that both the
above copyright notice(s) and this permission notice appear in supporting
documentation.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY
RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS
NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL
DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

Except as contained in this notice, the name of a copyright holder shall
not be used in advertising or otherwise to promote the sale, use or other
dealings in this Software without prior written authorization of the
copyright holder.

**************************************************************************/

#include "rnspace.hh"

using namespace RVL;

/** RVL Unit Test 1
    Purpose: exercise RnArray, StdSpace, RnSpace, and Vector 
    constructors, also RVLRandomize.
*/
    
int main() {

  try {
    // fix random seed to get same result every time
    //    srand(getpid());
    PlantSeeds(19490615);
    cout<<endl;
    cout<<"/*************************************************"<<endl;
    cout<<" *            BEGIN RVL UNIT TEST 1              *"<<endl;
    cout<<" * Purpose: exercise RnArray, StdSpace, RnSpace, *"<<endl;
    cout<<" * and Vector constructors, also RVLRandomize.   *"<<endl;
    cout<<" *************************************************/"<<endl; 
    cout<<endl;
    cout<<"1. construct 10-diml RnSpace<float>\n";
    RnSpace<float> sp(10);
    cout<<endl;
    cout<<"2. construct RVL::Vector in this space\n";
    Vector<float> x(sp);
    cout<<endl;
    cout<<"3. construct std::vector of RVL::Vector * cloning RVL::Vector\n";
    cout<<"   just constructed - exercises copy constructor\n";
    Vector<float> x0(x);
    Vector<float> x1(x);
    Vector<float> x2(x);
    Vector<float> x3(x);
    Vector<float> x4(x);
    vector< Vector<float> * > xx(5);
    xx[0]=&x0;
    xx[1]=&x1;
    xx[2]=&x2;
    xx[3]=&x3;
    xx[4]=&x4;
    cout<<endl;
    cout<<"4. construct RVLRandomize<float> FO\n";
    RVLRandomize<float> f;
    cout<<endl;
    cout<<"5. loop through components of std::vector<RVL::Vector>,\n";
    cout<<"   evaluating RVLRandomize FO on each, and printing it out\n";
    cout<<"   using write method of Vector class, which calls RnSpace\n";
    cout<<"   and RnArray write methods\n";
    for (int i=0;i<5;i++) {
      xx[i]->eval(f);
      cout<<endl;
      cout<<"*** component "<<i<<endl;
      xx[i]->write(cout);
    }
    cout<<endl;
    cout<<"6. use RVL::Vector::linComb to form 1.0 * 2nd component\n";
    cout<<"   plus 0.5 * 3rd component, store over first component,\n";
    cout<<"   write latter out.\n";
    cout<<endl;
    xx[0]->copy(*(xx[1]));
    float half = 0.5;
    xx[0]->linComb(half,*(xx[2]),ScalarFieldTraits<float>::One());
    xx[0]->write(cout);
    // test lin comb
    LocalVector<float> lx0(*(xx[0]));
    LocalVector<float> lx1(*(xx[1]));
    LocalVector<float> lx2(*(xx[2]));
    if ((lx1.getSize() != lx0.getSize())||
	(lx2.getSize() != lx0.getSize())) {
      RVLException e;
      e<<"Error: ut1\n";
      e<<"vects not same size\n";
      throw e;
    }
    for (int i=0;i<lx0.getSize();i++) {
      if (fabs((lx0.getData())[i] -
	       (lx1.getData())[i] - 0.5*
	       (lx2.getData())[i]) > 10*(numeric_limits<float>::epsilon())){
	RVLException e;
	e<<"Error: ut1\n";
	e<<"xx[0]["<<i<<"] != xx[1]["<<i<<"] + 0.5*xx[2]["<<i<<"]\n";
	throw e;
      }
    }
    cout<<endl;
    cout<<"/*************************************************"<<endl;
    cout<<" *              END RVL UNIT TEST 1              *"<<endl;
    cout<<" *************************************************/"<<endl; 
  }
  catch (RVLException & e) {
    e.write(cout);
    exit(1);
  }
}






