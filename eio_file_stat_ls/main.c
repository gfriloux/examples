#include <Eina.h>
#include <Ecore.h>
#include <Eio.h>

Eina_Bool
_filter(void *data EINA_UNUSED,
        Eio_File *handler EINA_UNUSED,
        const Eina_File_Direct_Info *info EINA_UNUSED)
{
   Eina_Stat es;
   Eina_Bool r;
   void *container;

   container = eio_file_container_get(handler);
   printf("(thread) handler[%p] container[%p]\n", handler, container);

   r = eina_file_statat(container, (Eina_File_Direct_Info *)info, &es);
   EINA_SAFETY_ON_TRUE_RETURN(r);

   printf("(thread) File %s : size=%llu mtime=%ld\n", info->path, es.size, es.mtime);

   return EINA_TRUE;
}

void
_main(void *data EINA_UNUSED,
      Eio_File *handler EINA_UNUSED,
      const Eina_File_Direct_Info *info)
{
   Eina_Stat es;
   Eina_Bool r;
   void *container;

   container = eio_file_container_get(handler);
   printf("(main) handler[%p] container[%p]\n", handler, container);

   r = eina_file_statat(container, (Eina_File_Direct_Info *)info, &es);
   EINA_SAFETY_ON_TRUE_RETURN(!r);

   printf("(main) File %s : size=%llu mtime=%ld\n", info->path, es.size, es.mtime);
}

void
_done(void *data EINA_UNUSED,
      Eio_File *handler EINA_UNUSED)
{
   ecore_main_loop_quit();
}

void
_error(void *data EINA_UNUSED,
       Eio_File *handler EINA_UNUSED,
       int error EINA_UNUSED)
{

}


int main(int argc, char **argv)
{
   eina_init();
   ecore_init();
   eio_init();

   eio_file_stat_ls("/home/", _filter, _main, _done, _error, NULL);
   ecore_main_loop_begin();

   eio_shutdown();
   ecore_shutdown();
   eina_shutdown();
   return 0;
}
