#include <Ecore.h>
#include <Azy.h>
#include "Demo_HowAreYou.azy_client.h"

#define CALL_CHECK(X) \
   do \
     { \
        if (!azy_client_call_checker(cli, err, ret, X, __PRETTY_FUNCTION__)) \
          { \
             printf("%s\n", azy_content_error_message_get(err)); \
             exit(1); \
          } \
     } while (0)
/* "debug" */

/**
 * Here we receive the response and print it
 */
static Eina_Error
_Demo_HowAreYou_Get_Ret(Azy_Client *client, Azy_Content *content, void *_response)
{
   Demo_HowAreYouResponse *response = _response;
   //retrieve the question, last argument passed to Demo_HowAreYou_Get()
   Demo_HowAreYouQuestion *question = azy_content_data_get(content);

   if (azy_content_error_is_set(content))
     {
        printf("Error encountered: %s\n", azy_content_error_message_get(content));
        azy_client_close(client);
        ecore_main_loop_quit();
        return azy_content_error_code_get(content);
     }

   //printf("%s: Success? %s!\n", __PRETTY_FUNCTION__, ret ? "YES" : "NO");
   printf("Question : %s\n", question->question);
   printf("Response : %s\n", response->response);

   Demo_HowAreYouQuestion_free(question);
   //response is automaticaly free
   return AZY_ERROR_NONE;
}

/**
 * Bad we have been disconnected
 */
static Eina_Bool _disconnected(void *data, int type, void *data2)
{
   ecore_main_loop_quit();
   return ECORE_CALLBACK_RENEW;
}

/**
 * Yes we are connected ! Now we can send our question
 */
static Eina_Bool
connected(void *data, int type, Azy_Client *cli)
{
   unsigned int ret;
   Azy_Content *content;
   Azy_Net *net;
   Azy_Content *err;
   Demo_HowAreYouQuestion *question;

   net = azy_client_net_get(cli);
   content = azy_content_new(NULL);
   err = azy_content_new(NULL);

   //send 3 questions
   //
   question=Demo_HowAreYouQuestion_new();
   question->question = eina_stringshare_add("How is Mystrale ?");
   ret = Demo_HowAreYou_Get(cli, question, content, question);
   CALL_CHECK(_Demo_HowAreYou_Get_Ret);
   //

   //
   question=Demo_HowAreYouQuestion_new();
   question->question = eina_stringshare_add("How is Fiona ?");
   ret = Demo_HowAreYou_Get(cli, question, content, question);
   CALL_CHECK(_Demo_HowAreYou_Get_Ret);
   //


   //
   question=Demo_HowAreYouQuestion_new();
   question->question = eina_stringshare_add("How is Sai ?");
   ret = Demo_HowAreYou_Get(cli, question, content, question);
   CALL_CHECK(_Demo_HowAreYou_Get_Ret);
   //

   azy_content_free(content);
   return ECORE_CALLBACK_RENEW;
}

int main(int argc, char *argv[])
{
   Ecore_Event_Handler *handler;
   Azy_Client *cli;

   eina_init();
   ecore_init();
   azy_init();

   /* create object for performing client connections */
   cli = azy_client_new();

   if (!azy_client_host_set(cli, "127.0.0.1", 3412))
     return 1;

   handler = ecore_event_handler_add(AZY_EVENT_CLIENT_CONNECTED, (Ecore_Event_Handler_Cb)connected, cli);
   handler = ecore_event_handler_add(AZY_EVENT_CLIENT_DISCONNECTED, (Ecore_Event_Handler_Cb)_disconnected, cli);

   /* connect to the servlet on the server specified by uri */
   if (!azy_client_connect(cli))
     return 1;

   ecore_main_loop_begin();

   azy_client_free(cli);
   azy_shutdown();
   ecore_shutdown();
   eina_shutdown();

   return 0;
}

