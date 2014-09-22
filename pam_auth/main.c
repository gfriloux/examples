#include <stdio.h>
#include <pwd.h>
#include <errno.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

char *_password = NULL;

static int
password_set(int num_msg,
             const struct pam_message **msg,
             struct pam_response **resp,
             void *appdata_ptr)
{
   *resp = calloc(num_msg, sizeof(struct pam_response));
   if (!*resp)
     {
        fprintf(stderr, "failed to allocate pam_response\n");
        return PAM_CONV_ERR;
     }

   (*resp)[0].resp = strdup(_password);
   (*resp)[0].resp_retcode = 0;

   return PAM_SUCCESS;
}

int main(int argc, char **argv)
{
   struct pam_conv conv = { password_set, NULL };
   pam_handle_t *pamh;
   struct passwd *pw;
   int r;

   if (argc < 3)
     {
        fprintf(stderr, "Usage : %s user password\n", argv[0]);
        return 1;
     }

   pw = getpwnam(argv[1]);
   if (!pw)
     {
        fprintf(stderr, "Failed to find user %s : %s\n", argv[1], strerror(errno));
        return 1;
     }

   _password = argv[2];

   r = pam_start("login", pw->pw_name, &conv, &pamh);
   if (r != PAM_SUCCESS)
     {
        fprintf(stderr, "pam_start failed : %s\n", pam_strerror(pamh, r));
        return 1;
     }

   r = pam_authenticate(pamh, 0);
   switch (r)
     {
      case PAM_USER_UNKNOWN:
        {
           fprintf(stderr, "User unknown.\n");
           break;
        }
      case PAM_SUCCESS:
        {
           fprintf(stdout, "Authentication successful.\n");
           break;
        }
      default:
        {
           fprintf(stderr, "Invalid password.\n");
           break;
        }
     }

   return 0;
}
