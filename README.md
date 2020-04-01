# NeoPepXMLParser
Library for reading, editing, and writing pepXML files.

Requires expat: https://libexpat.github.io/

Until I put together a Makefile, you're on your own for compiling.

No documentation yet, so here's the quick and dirty:

```cplusplus
#include "NeoPepXMLParser.h"
int main(){
 NeoPepXMLParser xml;
 xml.read("some.pep.xml");
 //change whatever you want by navigating xml.class.nextClass.etc...
 xml.write("new.pep.xml");
 return 0;
}
```
