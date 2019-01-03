#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdio.h>
#include <fcntl.h>

#include "my_socket.h"
#include "error_fail.h"

int create_socket_on_port( int port ) 
{
    int socket_fd;

    if ( ( socket_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) == 0 ) { 
        fail_on_error( "Can not create socket!" );
    } 

    struct sockaddr_in address; 
    memset(&address, '0', sizeof(address)); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 
    
    int sock_opt = 1;
    if ( setsockopt( socket_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &sock_opt, sizeof( sock_opt ) ) ) {
		fail_on_error( "Can not set socket options" ); 
    }
    
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    if ( bind( socket_fd, ( struct sockaddr * ) &address,  
                                 sizeof( address ) ) <0 ) { 
        fail_on_error( "Can not bind socket!" );
    } 

    return socket_fd;
}

void fd_linked_list_add_fd( struct fd_linked_list *fd_list_head, int new_fd ) 
{
    struct fd_linked_list *new_fd_node = ( struct fd_linked_list* ) malloc( 
                                            sizeof( struct fd_linked_list ) );

    if( new_fd_node == NULL ){
        fail_on_error("Unable to allocate memory for new fd node");
    }

    new_fd_node->fd = new_fd;
    new_fd_node->next = NULL;

    // check for first insertion
    if ( fd_list_head->next == NULL ){
        fd_list_head->next = new_fd_node;
        printf( "Node fd added to fd list." );
    }

    else
    {
        // else loop through the list and find the last
        // node, insert next to it
        struct fd_linked_list *fd_list_current = fd_list_head;
        while ( 1 ) {
            if( fd_list_current->next == NULL )
            {
                fd_list_current->next = new_fd_node;
                printf( "Node fd added to fd list." );
                break;
            }
            fd_list_current = fd_list_current->next;
        };
    }

    return;
}

void fd_linked_list_delete_fd( struct fd_linked_list *fd_list_head, int fd_to_delete ) 
{
    // Store head node 
    struct fd_linked_list* temp = fd_list_head, *prev; 
  
    // If head node itself holds the key to be deleted 
    if (temp != NULL && temp->fd == fd_to_delete) 
    { 
        fd_list_head = temp->next;   // Changed head 
        free(temp);               // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp->fd != fd_to_delete) 
    { 
        prev = temp; 
        temp = temp->next; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) return; 
  
    // Unlink the node from linked list 
    prev->next = temp->next; 
  
    free(temp);  // Free memory 
}