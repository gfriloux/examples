#include <Eina.h>
#include <Ecore.h>
#include <Azy.h>

Eina_Bool
_transfer(void *data,
          int type,
          void *ev)
{
   Azy_Event_Client_Transfer_Progress *dse = ev;
   char *d;
   size_t size;

   printf("Progress size=%zu current=%zu\n", dse->size, dse->current);

   d = azy_net_buffer_steal(dse->net, &size);
   if (!strlen(d))
     {
        printf("No data\n");
        return EINA_TRUE;
     }

   printf("d[%p]\n", d);
   printf("%s\n", d);

   return EINA_TRUE;
}

Eina_Bool
_disconnected(void *data,
              int type,
              void *ev)
{
   printf("_disconnected()\n");

   ecore_main_loop_quit();
   return EINA_TRUE;
}

Eina_Bool
_connected(void *data,
           int type,
           void *ev)
{
   Azy_Client *cli = data;
   Azy_Net *net;
   Azy_Content *content;
   Azy_Client_Call_Id id;

   printf("_connected()\n");


   net = azy_client_net_get(cli);
   content = azy_content_new(NULL);

   azy_net_uri_set(net, "/");
   azy_net_transport_set(net, AZY_NET_TRANSPORT_JSON);
   id = azy_client_blank(cli, AZY_NET_TYPE_GET, NULL, NULL, NULL);
   return EINA_TRUE;
}

int main(int argc, char **argv)
{
   Azy_Client *cli;
   Ecore_Event_Handler *handler;

   eina_init();
   ecore_init();
   azy_init();

   cli = azy_client_new();
   EINA_SAFETY_ON_NULL_RETURN_VAL(cli, 1);

   if (!azy_client_host_set(cli, argv[1], 80))
     return 1;

   handler = ecore_event_handler_add(AZY_EVENT_CLIENT_CONNECTED, _connected, cli);
   handler = ecore_event_handler_add(AZY_EVENT_CLIENT_DISCONNECTED, _disconnected, cli);
   handler = ecore_event_handler_add(AZY_EVENT_CLIENT_TRANSFER_PROGRESS, _transfer, cli);

   if (!azy_client_connect(cli))
     return 1;

   ecore_main_loop_begin();

   azy_shutdown();
   ecore_shutdown();
   eina_shutdown();

   return 0;
}
