#include <cnet.h>

/*  This function will be invoked each time the Application Layer has a
    message to deliver. Once we read the message from the application layer
    with CNET_read_application(), the Application Layer will be able to supply
    us with another message. The rate of new messages is controlled by
    the 'messagerate' node attribute.
 */

static EVENT_HANDLER(application_ready)
{
    CnetAddr	destaddr;
    char	buffer[MAX_MESSAGE_SIZE];
    size_t	length;

/*  Firstly, indicate the maximum sized message we are willing to receive */
    length = sizeof(buffer);

/*  Accept the message from the Application Layer.  We will be informed of the
    message's destination address and length and buffer will be filled in. */

    CHECK(CNET_read_application(&destaddr, buffer, &length));

    printf("\tI have a message of %4d bytes for address %d\n",
			    length, (int)destaddr);
    CHECK(CNET_write_physical(1, buffer, &length));
}

/* Function invoked each time a message is ready from physical layer */
static EVENT_HANDLER(physical_ready)
{
    // Determine whether the node running the code is the server node.
    // If so read message and write to application.
    if (nodeinfo.address == 200) {
        // Server behaviour.
        int link;
        char buffer[MAX_MESSAGE_SIZE];
        size_t length;
        length = sizeof(buffer);
         
        // Read physical layer
        CHECK(CNET_read_physical(&link, buffer, &length));

        // Write to the application layer.
        CNET_write_application(buffer, &length);
        printf("Server reads message of length %d\n", length);  
    }
     


}

static EVENT_HANDLER(button_pressed)
{
    printf("\n Node name       : %s\n",	nodeinfo.nodename);
    printf(" Node number     : %d\n",	nodeinfo.nodenumber);
    printf(" Node address    : %d\n",	nodeinfo.address);
    printf(" Number of links : %d\n\n",	nodeinfo.nlinks);
}

EVENT_HANDLER(reboot_node)
{
/*  Indicate that we are interested in hearing about the Application Layer
    having messages for delivery and indicate which function to invoke for them.
 */
    CNET_set_handler(EV_APPLICATIONREADY, application_ready, 0);
    CNET_set_handler(EV_PHYSICALREADY, physical_ready, 0);
    CNET_set_handler(EV_DEBUG0, button_pressed, 0);
    CNET_set_debug_string(EV_DEBUG0, "Node Info");
    
    // Only send messages to address 200
    CNET_enable_application(200);
}
