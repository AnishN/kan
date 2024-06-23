https://www.shamusyoung.com/twentysidedtale/?p=48683 (simple game explanation of why SOA can help)
https://asc.ziti.uni-heidelberg.de/sites/default/files/research/papers/public/St11ASX_CUDA.pdf (C++ implementation of AOS and SOA accessor class)
https://www.intel.com/content/www/us/en/developer/articles/technical/memory-layout-transformations.html (why SOA over AOS, as well as AOSOA)

https://indico.cern.ch/event/214784/contributions/1512874/attachments/341200/476108/AOSOA-CHEP-Public.pdf (intel presentation of ArrowStreet library)

Flexible representations with Arrow Street 
* table_vector<Cr,tablesize> v(len); // AOSOA
* table_vector<Cr,len> v(len); // SOA
* table_vector<Cr,1> v(len); // AOS
* std::vector<C> v(len); 

* table_array<Cr,tablesize,len> a; // AOSOA
* table_array<Cr,len,len> a; // SOA
* table_array<Cr,1,len> a; // AOS
* std::array<C,len> a;

How to do nesting though?

https://www.fluentcpp.com/2018/12/18/the-soa-vector-part-1-optimizing-the-traversal-of-a-collection/
https://www.fluentcpp.com/2018/12/21/an-soa-vector-with-an-stl-container-interface-in-cpp/ (C++ SOA impl)


Apache Arrow:
https://arrow.apache.org/faq/
https://arrow.apache.org/docs/format/Columnar.html
https://github.com/apache/arrow/tree/main/format