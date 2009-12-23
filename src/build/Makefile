######################################################################
# makefile for bp-tutorial-cpp1
# 22 Dec 2009
#
# builds libHello.so
#

SRVNAME= HelloWorld
SRCS= service.cpp
OBJS= $(SRCS:%.cpp=%.o)
OUTLIB= libHelloWorld.so
GPPCL= /usr/bin/g++-4.0
STAGEDIR= $(SRVNAME)

$(OUTLIB): service.o
	$(GPPCL) -isysroot /Developer/SDKs/MacOSX10.4u.sdk -O3 -DNDEBUG -bundle -headerpad_max_install_names  -o $(OUTLIB) $(OBJS) -F/Developer/SDKs/MacOSX10.4u.sdk/System/Library/Frameworks -framework Cocoa 
	mkdir $(STAGEDIR)
	cp $(OUTLIB) $(STAGEDIR)
	strip -x "$(STAGEDIR)/$(OUTLIB)"
	cp ../manifest_osx.json $(STAGEDIR)/manifest.json

#.cpp.o:
#	$(GPPCL) -c $(CXX_DEFINES) $(CXX_FLAGS) $< -o $@
service.o: ../service.cpp
	$(GPPCL) $(CXX_DEFINES) $(CXX_FLAGS) -o service.o -c ../service.cpp -I../../external/bp-service-framework/include

clean:
	rm service.o
	rm $(OUTLIB)
	rm -rf $(STAGEDIR)