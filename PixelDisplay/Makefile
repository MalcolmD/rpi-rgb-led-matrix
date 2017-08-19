CXXFLAGS=-Wall -O3 -g
OBJECTS=PixelDisplay.o PNGSampler.o
BINARIES=PixelDisplay PNG

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread



all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)




PixelDisplay: PixelDisplay.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) PixelDisplay.o -o $@ `libpng-config --ldflags` $(LDFLAGS)

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

PixelDisplay.o: PixelDisplay.cpp
	$(CXX) -I$(RGB_INCDIR) `libpng-config --cflags` $(CXXFLAGS) -c -o $@ $<





PNG: PNGSampler.o 
	$(CXX) $(CXXFLAGS) PNGSampler.o -o $@ `libpng-config --ldflags` 

PNGSampler.o: PNGSampler.cpp
	$(CXX) `libpng-config --cflags` $(CXXFLAGS) -c -o $@ $<






clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
