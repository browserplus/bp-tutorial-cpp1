//////////////////////////////
// HelloWorld.cpp 
//
// Demonstrates use of the Browserplus Service Framework to generate Browserplus
// services.
//
#include <sstream>
#include "bpservice/bpservice.h"

using namespace bplus::service;

class HelloWorld : public Service
{
public:    
    BP_SERVICE( HelloWorld );
    
    void greet( const Transaction& tran, const bplus::Map& args ) {
        std::stringstream ss;
        ss << "Hello, " << std::string(args["name"]) << "!";
        tran.complete( bplus::String( ss.str() ) );
    }
};

BP_SERVICE_DESC( HelloWorld, "HelloWorld", "1.0.0", "A simple Browserplus service" )
    ADD_BP_METHOD( HelloWorld, greet, "Generates a hearty greeting" )
      ADD_BP_METHOD_ARG( greet, "name", String, true, "name to greet" )
END_BP_SERVICE_DESC

