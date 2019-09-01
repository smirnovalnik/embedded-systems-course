void func(void)
{
    return;
}

void main(void)
{
    char i;
    
    i = 5;
    while (i > 0)
    {
        func();
        i--;
    }
    for (;;);
}
