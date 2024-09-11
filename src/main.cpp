/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuji <yuji@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:01:40 by ytoshihi          #+#    #+#             */
/*   Updated: 2024/09/11 10:22:15 by yuji             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		if (argc < 2)
			std::cerr << "\e[31minput value is needed!\e[0m" << std::endl;
		else
			std::cerr << "\e[31mToo many input\e[0m" << std::endl;
		return (1);
	}
	Config config(argv[1]);
	config.test();
	return (0);
}
