#include "ft_select.h"

void    open_window(t_lst_cir **lst_cir, int fd);
void    check_touch(t_lst_cir **lst_cir, int fd);

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

t_info  *singleton(t_info *data)
{
    static t_info  *info;

    if (data)
        info = data;
    return (info);
}

void    signalhandle(int signum)
{
    t_info  *info;

    (void)signum;
    info = singleton(NULL);
    ft_putstr_fd("TTTTTTTTTTTTTTTTTTTTTTTTTTTTT", info->fd);
    //open_window(info->lst_cir, info->fd);
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

void    check_touch(t_lst_cir **lst_cir, int fd)
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
    open_window(lst_cir, fd);
}

void    open_window(t_lst_cir **lst_cir, int fd)
{
    t_lst_cir       *tmp;
    int             x;
    int             y;
    struct winsize  w;

    x = 1;
    y = 1;
    ioctl(fd, TIOCGWINSZ, &w);
    if (*lst_cir == NULL)
        exit (0);
    ft_putstr_fd(tgetstr("cl", NULL), fd);
    if ((*lst_cir)->select == 1)
        ft_putstr_fd("\033[7m", fd);
    if ((*lst_cir)->curseur == 1)
        ft_putstr_fd("\033[4m", fd);
    ft_putstr_fd((*lst_cir)->content, fd);
    if (y == w.ws_row)
    {
        y = 0;
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
        if (y == w.ws_row)
        {
            y = 0;
            x += tmp->size_max;
        }
        move_to(x, y + 1, fd);
        y++;
        tmp = tmp->prev;
    }
    check_touch(lst_cir, fd);
}

int     main(int ac, char *av[])
{
    int             i;
    t_lst_cir       *lst_cir;
    t_lst_cir       *new;
    int             fd;
    int             len_max;
    t_info          *info;

    i = 1;
    len_max = 0;
    lst_cir = NULL;
    fd = 0;
    fd = open("/dev/tty", O_RDWR);
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
    if (!(info = (t_info*)ft_memalloc(sizeof(t_info))))
        return (0);
    info->fd = fd;
    info->lst_cir = &lst_cir;
    info = singleton(info);
    signal(SIGWINCH, &signalhandle);
    open_window(&lst_cir, fd);
    ft_putstr_fd(tgetstr("te", NULL), fd);
    ft_putstr_fd(tgetstr("ve", NULL), fd);
    return (0);
}