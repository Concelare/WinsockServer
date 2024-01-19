//
// Created by Ben on 09/01/2024.
//

#ifndef BLOGPOST_H
#define BLOGPOST_H
#include <vector>
#include <winsock2.h>


class BlogPost {
public:
    static void GetPosts(SOCKET socket);
    static void CreatePost(const BlogPost& blog_post);
    static void DeletePost(const char* post_name);

    BlogPost(char* title, char* description, char* creator);
private:
    // Holds All Posts
    static std::vector<BlogPost> Posts;

    char* title = nullptr;
    char* description = nullptr;
    char* creator = nullptr;
};



#endif //BLOGPOST_H
