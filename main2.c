#include "ft_select.h"

void    init_term(void)
{
    struct termios  attr;

    tcgetattr(STDIN_FILENO, &attr);
    attr.c_lflag &= ~(ECHO | ICANON);
    attr.c_oflag &= ~(OPOST);
    attr.c_cc[VMIN] = 1;
    attr.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &attr);
    tgetent(NULL, getenv("TERM"));
}

void    default_term(void)
{
    struct termios  attr;

    tcgetattr(STDIN_FILENO, &attr);
    attr.c_lflag |= (ECHO | ICANON);
    attr.c_oflag |= (OPOST);
    tcsetattr(STDIN_FILENO, TCSADRAIN, &attr);
    tgetent(NULL, getenv("TERM"));
}

int    test(int c)
{
    write(1, &c, 1);
    return (1);
}

void    treatment(t_lst_cir **lst_cir, int fd)
{
    tputs(tgetstr("ti", NULL), 1, &test);
    //tputs(tgetstr("cl", NULL), 1, &test);
    ft_putlstcir_fd(*lst_cir, fd);
    voir_touche(lst_cir, fd);
}

int     voir_touche(t_lst_cir **lst_cir, int fd)
{
  char      buffer[4];
  t_lst_cir *tmp;
 
  while (1)
  {
    bzero(buffer, 4);
    read(0, buffer, 3);
    if (buffer[0] == 27 && buffer[1] == '[' && buffer[2] == 'B')
    {
        tmp = *lst_cir;
        tputs(tgetstr("te", NULL), 1, &test);
        while (tmp->curseur != 1)
            tmp = tmp->prev;
        tmp->prev->curseur = 1;
        tmp->curseur = 0;
        treatment(lst_cir, fd);
    }
    else if (buffer[0] == 27 && buffer[1] == '[' && buffer[2] == 'A')
    {
        tmp = *lst_cir;
        tputs(tgetstr("te", NULL), 1, &test);
        while (tmp->curseur != 1)
            tmp = tmp->next;
        tmp->next->curseur = 1;
        tmp->curseur = 0;
        treatment(lst_cir, fd);
    }
    else if (buffer[0] == 4)
    {
        default_term();
        exit(0);
    }
  }
  return (0);
}

int     main(int ac, char *av[])
{
    int         i;
    t_lst_cir   *lst_cir;
    t_lst_cir   *new;
    int         fd;

    fd = 0;
    fd = open("/dev/tty", O_RDWR);
    i = 1;
    lst_cir = NULL;
    init_term();
    while (i < ac)
    {
        new = create_lst_cir(av[i]);
        add_next_lst_cir(&lst_cir, new);
        i++;
    }
    t_lst_cir   *tmp;
    printf("%s", lst_cir->content);
    tmp = lst_cir->prev;
    while (tmp != lst_cir)
    {
        tputs(tgetstr("cr", NULL), 1, &test);
        printf("%s", tmp->content);
        tmp = tmp->prev;
    }
    /*if (lst_cir)
        lst_cir->curseur = 1;*/
    treatment(&lst_cir, fd);
}