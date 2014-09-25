#include <Eina.h>
#include <Ecore.h>
#include <Ecore_Con.h>
#include <unistd.h>

typedef struct _Example
{
   Ecore_Fd_Handler *fd;
   Ecore_Con_Server *svr;

   Eina_Bool connected;

   struct
   {
      Ecore_Event_Handler *add,
                          *del,
                          *dat;
   } ev;
} Example;

Eina_Bool
_add(void *data,
     int type EINA_UNUSED,
     void *ev EINA_UNUSED)
{
   Example *ex = data;

   printf("Connected!\n");
   sleep(1);
   printf("\e[1;1H\e[2J");
   fflush(stdout);

   ex->connected = EINA_TRUE;

   return EINA_TRUE;
}

Eina_Bool
_del(void *data,
     int type EINA_UNUSED,
     void *ev EINA_UNUSED)
{
   Example *ex = data;

   printf("Disconnected!\n");

   ex->connected = EINA_FALSE;
   ex->svr = NULL;
   ecore_main_loop_quit();
   return EINA_TRUE;
}

Eina_Bool
_dat(void *data EINA_UNUSED,
     int type EINA_UNUSED,
     void *ev)
{
   Ecore_Con_Event_Server_Data *d = ev;

   printf("%.*s", d->size, d->data);

   return EINA_TRUE;
}

Eina_Bool
_fd(void *data,
    Ecore_Fd_Handler *handler)
{
   Example *ex = data;
   Eina_Bool r;
   int fd;
   char buf[1024];
   size_t len;

   if (!ex->connected)
     return EINA_TRUE;

   r = ecore_main_fd_handler_active_get(handler, ECORE_FD_ERROR);
   EINA_SAFETY_ON_TRUE_GOTO(r, fd_error);

   fd = ecore_main_fd_handler_fd_get(handler);

   len = read(fd, buf, sizeof(buf));
   if (!len)
     return EINA_TRUE;

   //printf("%.*s\n", len, buf);

   ecore_con_server_send(ex->svr, buf, len);
   ecore_con_server_flush(ex->svr);

   return EINA_TRUE;

fd_error:
   printf("An error occurred while reading stdin.\n");
   ecore_main_loop_quit();
   return EINA_FALSE;
}

int main(int argc, char **argv)
{
   Example *ex;

   if (argc < 3)
     {
        printf("Usage : %s hostname port\n", argv[0]);
        return 1;
     }

   eina_init();
   ecore_init();
   ecore_con_init();

   ex = calloc(1, sizeof(Example));
   EINA_SAFETY_ON_NULL_GOTO(ex, shutdown_ecore_con);

   ex->fd = ecore_main_fd_handler_add(STDIN_FILENO,
                                      ECORE_FD_READ | ECORE_FD_ERROR,
                                      _fd, ex, NULL, NULL);
   EINA_SAFETY_ON_NULL_GOTO(ex->fd, free_ex);

   ex->svr = ecore_con_server_connect(ECORE_CON_REMOTE_TCP, argv[1],
                                      atoi(argv[2]), NULL);
   EINA_SAFETY_ON_NULL_GOTO(ex->svr, free_fd);

   ex->ev.add = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_ADD, _add, ex);
   EINA_SAFETY_ON_NULL_GOTO(ex->ev.add, free_svr);

   ex->ev.del = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DEL, _del, ex);
   EINA_SAFETY_ON_NULL_GOTO(ex->ev.del, del_add);

   ex->ev.dat = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, _dat, ex);
   EINA_SAFETY_ON_NULL_GOTO(ex->ev.dat, del_del);

   printf("Connecting to %s on port %s\n", argv[1], argv[2]);

   ecore_main_loop_begin();

   ecore_event_handler_del(ex->ev.dat);
del_del:
   ecore_event_handler_del(ex->ev.del);
del_add:
   ecore_event_handler_del(ex->ev.add);
free_svr:
   if (ex->svr) ecore_con_server_del(ex->svr);
free_fd:
   ecore_main_fd_handler_del(ex->fd);
free_ex:
   free(ex);
shutdown_ecore_con:
   ecore_con_shutdown();
   ecore_shutdown();
   eina_shutdown();
   return 0;
}
