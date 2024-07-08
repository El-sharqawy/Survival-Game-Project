//
// Created by osama on 07/07/24.
//

#ifndef UTILS_H
#define UTILS_H

template <typename TContainer>
void stl_wipe(TContainer& container)
{
	for (auto con = container.begin(); con != container.end(); con++)
	{
		delete *con;
		*con = nullptr;
	}

	container.clear();
}

#endif //UTILS_H
