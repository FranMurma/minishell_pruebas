/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmurcia <frmurcia@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:27:15 by frmurcia          #+#    #+#             */
/*   Updated: 2023/07/06 12:58:03 by frmurcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../inc/minishell.h"
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>

int	ft_strlen(char *str)
{
	int	c;

	c = 0;
	while (str[c])
		c++;
	return (c);
}

bool	is_quote(char c)
{
	if (c == '"' || c == '\'')
		return (true);
	return (false);
}

// Para el ft_substr -->  para el ft_trim
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize)
	{
		if (dstsize == 1)
			dst[0] = '\0';
		else
		{
			while (src[i] && i <= dstsize - 2)
			{
				dst[i] = src[i];
				i++;
			}
			dst[i] = '\0';
		}
	}
	while (src[i])
		i++;
	return (i);
}

// para el ft_trim
char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	str_l;
	size_t	l;

	str_l = ft_strlen(s);
	if (str_l - start >= len)
		l = len + 1;
	else
		l = str_l - start + 1;
	if (len == 0 || str_l == 0 || start > str_l)
	{
		str = malloc(sizeof(char));
		if (str == NULL)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	str = malloc(sizeof(char) * l);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, (s + start), l);
	return (str);
}

bool	empty_space(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

//Para el ft_trim. Nos da el punto en el que empieza a haber algo que
//no son espacios vacios en la cadena
int	trim_start(char *str)
{
	int	c;

	c = 0;
	while (str[c] && empty_space(str[c]))
		c++;
	return (c);
}

//Para el ft_trim. Nos da la longitud de la cadena sin espacios
//por delante ni por detras. Necesario para hacer una cadena nueva
//sin espacios delante ni detras
int	trim_len(char *str)
{
	int	c;
	int	start;

	start = trim_start(str);
	c = ft_strlen(str) - 1;
	while (c > 0 && empty_space(str[c]))
		c--;
	return (c - start + 1);
}

//Para el remove_sp. Nos da la longitud de la nueva cadena sin espacios dobles
int	strlen_nosp(char *str)
{
	int		c;
	int		i;
	char	quote;
	bool	quoted_on;

	c = 0;
	i = 0;
	quote = '\0';
	quoted_on = false;
	while (str[c])
	{
		if (!empty_space(str[c]))
		{
			if (is_quote(str[c]) && quoted_on == false)
			{
				quoted_on = true;
				quote = str[c];
			}
			else if (is_quote(str[c]) && quoted_on == true && quote == str[c])
			{
				quoted_on = false;
				quote = '\0';
			}
			i++;
		}
		else if (empty_space(str[c]) && quoted_on == true)
			i++;
		else if (str[c + 1] && i != 0 && empty_space(str[c]) && quoted_on == false && !empty_space(str[c + 1]) && str[c + 1] != '\0')
			i++;
		c++;
	}
//	printf("El len de la cadena sin espacios es: %i\n", i);
	return (i);
}

//Nos da la cadena nueva sin espacios dobles (o mas)
char	*remove_sp(char *str)
{
	int		c;
	int		i;
	bool	quoted_on;
	char	quote;
	char	*new_str;

	i = 0;
	c = 0;
	quoted_on = false;

	new_str = malloc(sizeof(char) * (strlen_nosp(str) + 1));
	while (str[c])
	{
		if (!empty_space(str[c]))
		{
			if (is_quote(str[c]) && quoted_on == false)
			{
				quoted_on = true;
				quote = str[c];
			}
			else if (is_quote(str[c]) && quoted_on == true && quote == str[c])
			{
				quoted_on = false;
				quote = str[c];
			}
			new_str[i] = str[c];
			i++;
		}
		else if (empty_space(str[c]) && quoted_on == true)
		{
			new_str[i] = ' ';
			i++;
		}
		else if (str[c + 1] && i != 0 && empty_space(str[c]) && quoted_on == false && !empty_space(str[c + 1]) && str[c + 1] != '\0')
		{
			new_str[i] = ' ';
			i++;
		}
		c++;
	}
	str[i] = '\0';
	if (new_str != NULL)
		free (str);
	return (new_str);
}

char	*ft_trim(char *str)
{
	int		start;
	int		len;

	char	*trim_str;
	char	*new_str;
	start = trim_start(str);
	len = trim_len(str);
	trim_str = ft_substr(str, start, len);
	new_str = remove_sp(trim_str);
	
	return (new_str);
}

bool	quotes_ok(char *str)
{
	int		c;
	char	quote;
	bool	quoted_off;

	c = 0;
	quote = '\0';
	quoted_off = true;
	while (str[c])
	{
		if (is_quote(str[c]) && quoted_off)
		{
			quoted_off = false;
			quote = str[c];
		}
		else if (is_quote(str[c]) && !quoted_off && str[c] == quote)
		{
			quoted_off = true;
			quote = '\0';
		}
		c++;
	}
	if (quoted_off == true)
		return (true);
	return (false);
}

int	main(void)
{
	char	*new_str1;
	char    *new_str2;
	char    *new_str3;
	char    *new_str4;
	char	*new_str5;
	char	*new_str6;
	char    *new_str7;
	char    *new_str8;
	char    *str1 = " Hola 		'a' ";
	char	*str2 = "    	Hola 	 	 que		 tal	  ";
	char    *str3 = "  Hola  que      a      tal      ";
	char    *str4 = "Hola 'que            tal      ";
	char	*str5 = "";
	char	*str6 = "''";
	char	*str7 = "hola";
	char	*str8 = "'hola'";
	bool	a;
	bool	b;
	bool	c;
	bool	d;
	bool	e;
	bool	f;

	new_str1 = ft_trim(str1);
	new_str2 = ft_trim(str2);
	new_str3 = ft_trim(str3);
	new_str4 = ft_trim(str4);
	new_str5 = ft_trim(str5);
	new_str6 = ft_trim(str6);
	new_str7 = ft_trim(str7);
	new_str8 = ft_trim(str8);
	printf("Esta es la cadena: -%s-\n", str1);
	printf("Esta es la cadena recortada: -%s-\n\n", new_str1);
	printf("Esta es la cadena: -%s-\n", str2);
	printf("Esta es la cadena recortada: -%s-\n\n", new_str2);
	printf("Esta es la cadena: -%s-\n", str3);
	printf("Esta es la cadena recortada: -%s-\n\n", new_str3);
	printf("Esta es la cadena: -%s-\n", str4);
	printf("Esta es la cadena recortada: -%s-\n\n", new_str4);
	a = quotes_ok(str1);
	if (a == true)
		printf("En la primera cadena los parentesis se cierran\n");
	else if (a == false)
		printf("En la primera cadena los parentesis estan mal\n");
	b = quotes_ok(str4);
	if (b == true)
		printf("En la cuarta cadena los parentesis se cierran\n");
	else if (b == false)
		printf("En la cuarta cadena los parentesis estan mal\n");
	printf("\nCadena 5 : %s\n", str5);
	printf("char    *str5 = \"\";\n");
	printf("Cadena 5 recortada: %s\n", new_str5);
	c = quotes_ok(str5);
	if (c == true)
		printf("En la cadena 5 los parentesis se cierran bien\n");
	else if (c == false)
		printf("En la cadena 5 los parentesis estan mal\n");
	printf("\nCadena 6 : %s\n", str6);
	printf("char    *str6 = \"''\";\n");
	d = quotes_ok(str6);
	if (d == true)
		printf("En la cadena 6 los parentesis se cierran bien\n");
	else if (d == false)
		printf("En la cadena 6 los parentesis estan mal\n");
	printf("Cadena 6 recortada: %s\n", new_str6);
	printf("\nCadena 7 : %s\n", str7);
	printf("char    *str7 = \"hola\";\n");
	printf("Cadena 7 recortada: %s\n", new_str7);
	e = quotes_ok(str7);
	if (e == true)
		printf("En la cadena 7 los parentesis se cierran bien\n");
	else if (e == false)
		printf("En la cadena 7 los parentesis estan mal\n");
	printf("\nCadena 8 : %s\n", str8);
	printf("char    *str8 = \"'hola'\";\n");
	printf("Cadena 8 recortada: %s\n", new_str8);
	f = quotes_ok(str8);
	if (f == true)
		printf("En la cadena 8 los parentesis se cierran bien\n");
	else if (d == false)
		printf("En la cadena 8 los parentesis estan mal\n");

}
