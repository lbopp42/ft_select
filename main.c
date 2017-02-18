#include "ft_select.h"

void    open_window(t_lst_cir **lst_cir, int fd, int w_size);

void    init_term(void)
{
    struct termios  attr;

    tcgetattr(STDIN_FILENO, &attr);
    attr.c_lflag &= ~(ECHO | ICANON);
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
    tcsetattr(STDIN_FILENO, TCSADRAIN, &attr);
    tgetent(NULL, getenv("TERM"));
}

int    test(int c)
{
    write(1, &c, 1);
    return (1);
}

void    move_to(int x, int y, int fd)
{
    char    *s;

    s = ft_strjoin("\033[", ft_itoa(y));
    s = ft_stradd(s, ";");
    s = ft_stradd(s, ft_itoa(x));
    s = ft_stradd(s, "H");
    ft_putstr_fd(s, fd);
    free(s);
}

void    check_touch(t_lst_cir **lst_cir, int fd, int w_size)
{
    char        buffer[4];
    t_lst_cir   *tmp;

    bzero(buffer, 4);
    read(0, buffer, 4);
    tmp = *lst_cir;
    while (tmp->curseur != 1)
        tmp = tmp->prev;
    if (buffer[0] == 4)
    {
        default_term();
        ft_putstr_fd(tgetstr("te", NULL), fd);
        ft_putstr_fd(tgetstr("ve", NULL), fd);
        close(fd);
        exit(0);
    }
    else if (buffer[0] == 27 && buffer[1] == 91 && buffer[2] != 51)
    {
        if (buffer[2] == 66)
        {
            tmp->curseur = 0;
            tmp->prev->curseur = 1;
        }
        if (buffer[2] == 65)
        {
            while (tmp->curseur != 1)
                tmp = tmp->prev;
            tmp->curseur = 0;
            tmp->next->curseur = 1;
        }
    }
    else if (buffer[0] == 32)
    {
        tmp->curseur = 0;
        tmp->select = tmp->select ? 0 : 1;
        tmp->prev->curseur = 1;
    }
    else if (buffer[0] == 127 || (buffer[0] == 27
        && buffer[1] == 91 && buffer[2] == 51 && buffer[3] == 126))
    {
        if (tmp == *lst_cir)
            *lst_cir = (*lst_cir)->prev;
        if (tmp->prev == tmp)
        {
            tmp = del_one_lst_cir(tmp);
            default_term();
            ft_putstr_fd(tgetstr("te", NULL), fd);
            ft_putstr_fd(tgetstr("ve", NULL), fd);
            close(fd);
            exit(0);
        }
        tmp = del_one_lst_cir(tmp);
    }
    else if (buffer[0] == '\n')
    {
        default_term();
        ft_putstr_fd(tgetstr("te", NULL), fd);
        ft_putstr_fd(tgetstr("ve", NULL), fd);
        close(fd);
        print_select_lst_cir(*lst_cir);
        exit(0);   
    }
    open_window(lst_cir, fd, w_size);
}

void    open_window(t_lst_cir **lst_cir, int fd, int w_size)
{
    t_lst_cir   *tmp;
    int         x;
    int         y;

    x = 1;
    y = 1;
    if (*lst_cir == NULL)
        exit (0);
    ft_putstr_fd(tgetstr("cl", NULL), fd);
    ft_putstr_fd("OK", fd);
    if ((*lst_cir)->select == 1)
        ft_putstr_fd("\033[7m", fd);
    if ((*lst_cir)->curseur == 1)
        ft_putstr_fd("\033[4m", fd);
    ft_putstr_fd((*lst_cir)->content, fd);
    if (y == w_size)
    {
        y = 1;
        x += (*lst_cir)->size_max;
    }
    move_to(x, y + 1, fd);
    y++;
    ft_putstr_fd("\033[0m", fd);
    tmp = (*lst_cir)->prev;
    while (tmp != *lst_cir)
    {
        if (tmp->select == 1)
            ft_putstr_fd("\033[7m", fd);
        if (tmp->curseur == 1)
            ft_putstr_fd("\033[4m", fd);
        ft_putstr_fd(tmp->content, fd);
        ft_putstr_fd("\033[0m", fd);
        ft_putstr_fd(ft_itoa(w_size), fd);
        ft_putstr_fd(ft_itoa(y), fd);
        if (y == w_size)
        {
            y = 0;
            x += tmp->size_max;
        }
        ft_putstr_fd(ft_itoa(x), fd);
        move_to(x, y + 1, fd);
        y++;
        tmp = tmp->prev;
    }
    check_touch(lst_cir, fd, w_size);
}

int     main(int ac, char *av[])
{
    int             i;
    t_lst_cir       *lst_cir;
    t_lst_cir       *new;
    int             fd;
    int             len_max;
    struct winsize  w;

    i = 1;
    len_max = 0;
    lst_cir = NULL;
    fd = 0;
    fd = open("/dev/tty", O_RDWR);
    ioctl(fd, TIOCGWINSZ, &w);
    init_term();
    while (i < ac)
    {
        if (len_max < ft_strlen(av[i]))
            len_max = ft_strlen(av[i]);
        i++;
    }
    i = 1;
    while (i < ac)
    {
        new = create_lst_cir(av[i], len_max);
        add_next_lst_cir(&lst_cir, new, len_max);
        i++;
    }
    ft_putstr_fd(tgetstr("ti", NULL), fd);
    ft_putstr_fd(tgetstr("vi", NULL), fd);
    lst_cir->curseur = 1;
    //signal(SIGWINCH, &open_window);
    open_window(&lst_cir, fd, w.ws_row);
    ft_putstr_fd(tgetstr("te", NULL), fd);
    ft_putstr_fd(tgetstr("ve", NULL), fd);
    return (0);
}