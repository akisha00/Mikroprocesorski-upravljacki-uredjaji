#define n 4

unsigned char str2num(char *str)
{
    unsigned char ret_val = 0;
    unsigned char i = 0;
    unsigned char j = 1;

    switch (*str)
    {

    case 'b':

        i++;
        while (*(str + i) != '\0')
        {
            i++;
        }

        i--;

        while (i)
        {
            ret_val += (*(str + i) - 48) * j;
            j = j << 1;
            i--;
        }

        return ret_val;

    case 'H':

        i++;
        while (*(str + i) != '\0')
        {
            i++;
        }

        i--;

        while (i)
        {
            if (*(str + i) - 48 <= 9)
            {
                ret_val += (*(str + i) - 48) * j;
                j = j << 4;
                i--;
            }
            else
            {
                ret_val += (*(str + i) - 55) * j;
                j = j << 4;
                i--;
            }
        }

        return ret_val;

    default:

        while (*(str + i) != '\0')
        {
            i++;
        }

        i--;

        while (i)
        {
            ret_val += (*(str + i) - 48) * j;
            j *= 10;
            i--;
        }

        return ret_val + ((*(str + i) - 48) * j);
    }
}

void num2str(unsigned char num, char *rez)
{
    unsigned char i = 0, j = 0, temp = num;
    while (num)
    {
        temp = num % 10;
        *(rez + i) = temp + 48;
        num /= 10;
        i++;
    }
    rez[i] = '\0';
    i--;
    while (i)
    {
        temp = *(rez + i);
        *(rez + i) = *(rez + j);
        *(rez + j) = temp;
        i--;
        j++;
    }
}
void izracunaj(char *op1, char *op2, char *rez, char operant)
{
    unsigned char opn1 = str2num(op1);
    unsigned char opn2 = str2num(op2);
    unsigned char opnr;

    switch (operant)
    {
    case 43:
        opnr = opn1 + opn2;
        break;

    case 47:
        opnr = opn1 / opn2;
        break;

    default:
        opnr = opn1 * opn2;
        break;
    }

    num2str(opnr, rez);
}

void main(void)
{

    char *xd1 = "32";
    char *xd2 = "8";
    char xd3[n];
    izracunaj(xd1, xd2, xd3, '+');

	while(1)
	{
	}
}