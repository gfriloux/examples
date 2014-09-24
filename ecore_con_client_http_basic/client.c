#include <stdio.h>
#include <Eina.h>
#include <Ecore.h>
#include <Ecore_Con.h>

char *_host = NULL;

Eina_Bool
_add(void *data EINA_UNUSED, int type EINA_UNUSED, Ecore_Con_Event_Server_Add *ev)
{
   char query[512];

   printf("Server with ip %s connected!\n", ecore_con_server_ip_get(ev->server));


   sprintf(query, "GET / HTTP/1.1\r\n"
                  "host: %s\r\n\r\n",
           _host);

   ecore_con_server_send(ev->server, query, strlen(query));
   ecore_con_server_flush(ev->server);

   return ECORE_CALLBACK_RENEW;
}

Eina_Bool
_del(void *data EINA_UNUSED, int type EINA_UNUSED, Ecore_Con_Event_Server_Del *ev)
{
   printf("Lost server with ip %s!\n", ecore_con_server_ip_get(ev->server));
   ecore_main_loop_quit();
   return ECORE_CALLBACK_RENEW;
}

Eina_Bool
_data(void *data EINA_UNUSED, int type EINA_UNUSED, Ecore_Con_Event_Server_Data *ev)
{
   char fmt[128];

   snprintf(fmt, sizeof(fmt),
            "Received %i bytes from server:\n"
            ">>>>>\n"
            "%%.%is\n"
            ">>>>>\n",
            ev->size, ev->size);

   printf(fmt, ev->data);
   return ECORE_CALLBACK_RENEW;
}

int
main(int argc, char **argv)
{
   Ecore_Con_Server *svr;

   eina_init();
   ecore_init();
   ecore_con_init();

   if (argc < 2)
     return 1;

   printf("Connection to %s\n", argv[1]);
   if (!(svr = ecore_con_server_connect(ECORE_CON_REMOTE_TCP, argv[1], 80, NULL)))
     exit(1);

   _host = argv[1];

   ecore_event_handler_add(ECORE_CON_EVENT_SERVER_ADD, (Ecore_Event_Handler_Cb)_add, NULL);
   ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DEL, (Ecore_Event_Handler_Cb)_del, NULL);
   ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, (Ecore_Event_Handler_Cb)_data, NULL);

   ecore_main_loop_begin();

   return 0;
}

