#include <Strukt/strukt.h>
#include <Strukt/logger.h>

int main()
{
    log_info("Hello World!");

    // Set up strukt instance
    struct strukt_instance my_instance;
    struktReturnDefaultInstance(&my_instance);

    // Run strukt engine
    struktRun(&my_instance);
    return 0;
}
