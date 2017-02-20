#include "ft_select.h"

t_info  *singleton(t_info *data);
void    draw_window(t_lst_cir **lst_cir, int fd);

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

void    signalhandle(int signum)
{
    t_info  *info;

    (void)signum;
    info = singleton(NULL);
    draw_window(info->lst_cir, info->fd);
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

int     count_len_max(int ac, char **av)
{
    int i;
    int len_max;

    i = 1;
    len_max = 0;
    while (i < ac)
    {
        if (len_max < ft_strlen(av[i]))
            len_max = ft_strlen(av[i]);
        i++;
    }
    return (len_max);
}

t_info  *singleton(t_info *data)
{
    static t_info  *info;

    if (data)
        info = data;
    return (info);
}

void    init_sigleton(t_lst_cir **lst_cir, int fd)
{
    t_info          *info;

    if (!(info = (t_info*)ft_memalloc(sizeof(t_info))))
         return ;
    info->fd = fd;
    info->lst_cir = lst_cir;
    info = singleton(info);
}

void    draw_first_elem(t_lst_cir **lst_cir, int fd, int *x, int *y)
{
    struct winsize  w;

    ioctl(fd, TIOCGWINSZ, &w);
    if ((*lst_cir)->select == 1)
        ft_putstr_fd("\033[7m", fd);
    if ((*lst_cir)->curseur == 1)
        ft_putstr_fd("\033[4m", fd);
    ft_putstr_fd((*lst_cir)->content, fd);
    if (*y == w.ws_row)
    {
        *y = 0;
        *x += (*lst_cir)->size_max;
    }
    move_to(*x, *y + 1, fd);
    *y += 1;
    ft_putstr_fd("\033[0m", fd);
}

void    draw_other_elem(t_lst_cir **lst_cir, int fd, int *x, int *y)
{
    t_lst_cir   *tmp;
    struct winsize  w;

    ioctl(fd, TIOCGWINSZ, &w);
    tmp = (*lst_cir)->prev;
    while (tmp != *lst_cir)
    {
        if (tmp->select == 1)
            ft_putstr_fd("\033[7m", fd);
        if (tmp->curseur == 1)
            ft_putstr_fd("\033[4m", fd);
        ft_putstr_fd(tmp->content, fd);
        ft_putstr_fd("\033[0m", fd);
        if (*y == w.ws_row)
        {
            *y = 0;
            *x += tmp->size_max;
        }
        move_to(*x, *y + 1, fd);
        *y += 1;
        tmp = tmp->prev;
    }
}

void    draw_window(t_lst_cir **lst_cir, int fd)
{
    int             x;
    int             y;

    x = 1;
    y = 1;
    ft_putstr_fd(tgetstr("cl", NULL), fd);
    draw_first_elem(lst_cir, fd, &x, &y);
    draw_other_elem(lst_cir, fd, &x, &y);
}

void    exit_term(t_lst_cir *lst_cir ,int fd, char buffer[])
{
    default_term();
    ft_putstr_fd(tgetstr("te", NULL), fd);
    ft_putstr_fd(tgetstr("ve", NULL), fd);
    close(fd);
    if (buffer[0] == '\n')
        print_select_lst_cir(lst_cir);
    exit(0);
}

void    check_key(t_lst_cir **lst_cir, int fd)
{
    char        buffer[4];
    t_lst_cir   *tmp;

    while (42)
    {
        ft_bzero(buffer, 4);
        read(0, buffer, 4);
        tmp = *lst_cir;
        while (tmp->curseur != 1)
            tmp = tmp->prev;
        (buffer[0] == 4 || buffer[0] == '\n') ? exit_term(*lst_cir, fd, buffer) : 0;
        if (buffer[0] == 27 && buffer[1] == 91 &&
            (buffer[2] == 66 || buffer[2] == 65))
        {
            tmp->curseur = 0;
            tmp->prev->curseur = (buffer[2] == 66) ? 1 : 0;
            tmp->next->curseur = (buffer[2] == 65) ? 1 : 0;
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
                exit_term(*lst_cir, fd, buffer);
            }
            tmp = del_one_lst_cir(tmp);
        }
        draw_window(lst_cir, fd);
    }
}

void    fill_list_arg(int ac, char **av, int len_max, t_lst_cir **lst_cir)
{
    int         i;
    t_lst_cir   *new;

    i = 1;
    new = NULL;
    while (i < ac)
    {
        new = create_lst_cir(av[i], len_max);
        add_next_lst_cir(lst_cir, new, len_max);
        i++;
    }
}

int     main(int ac, char *av[])
{
    t_lst_cir       *lst_cir;
    int             fd;
    int             len_max;

    if (ac < 2)
        return (0);
    len_max = 0;
    lst_cir = NULL;
    fd = 0;
    fd = open("/dev/tty", O_RDWR);
    init_term();
    len_max = count_len_max(ac, av);
    fill_list_arg(ac, av, len_max, &lst_cir);
    ft_putstr_fd(tgetstr("ti", NULL), fd);
    ft_putstr_fd(tgetstr("vi", NULL), fd);
    lst_cir->curseur = 1;
    init_sigleton(&lst_cir, fd);
    signal(SIGWINCH, &signalhandle);
    draw_window(&lst_cir, fd);
    check_key(&lst_cir, fd);
    ft_putstr_fd(tgetstr("te", NULL), fd);
    ft_putstr_fd(tgetstr("ve", NULL), fd);
    return (0);
}