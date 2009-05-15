
rm -f *.o *.so


gcc -I. -I/usr/local/share/asn1c/ -fPIC -c -g *.c 

gcc -shared -Wl,-soname,libCGF_BERHelper.so -o libCGF_BERHelper.so *.o





