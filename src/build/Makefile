######################################################################
# makefile for bp-tutorial-cpp1
# 22 Dec 2009
#
# builds libHello.so
#

SRVNAME= HelloWorld
SRCS= service.cpp
OBJS= $(SRCS:%.cpp=%.o)
OUTLIB= libHello.so
GPPCL= /usr/bin/g++-4.0
STAGEDIR= $(SRVNAME)

.cpp.o:
	$(GPPCL) -c $(CXX_DEFINES) $(CXX_FLAGS) $< -o $@

$(OUTLIB): $(OBJS)
    $(GPPCL) -isysroot /Developer/SDKs/MacOSX10.4u.sdk -O3 -DNDEBUG -bundle -headerpad_max_install_names  -o $(OUTLIB) $(OBJS) -F/Developer/SDKs/MacOSX10.4u.sdk/System/Library/Frameworks -framework Cocoa 
	cp $(OUTLIB) $(STAGEDIR)
	strip -x "$(STAGEDIR)/$(OUTLIB)"
