//
// Created by Ben on 09/01/2024.
//

#include "BlogPost.h"

// Sends All Posts to The Client
void BlogPost::GetPosts(const SOCKET socket) {
    send(socket, reinterpret_cast<char*>(&Posts), sizeof(Posts), 0);
}

void BlogPost::CreatePost(const BlogPost& blog_post) {
    // Adds Post To Posts
    Posts.push_back(blog_post);
}

void BlogPost::DeletePost(const char* post_name) {
    // Initialises Counter
    int i = 0;
    // Searches For Posts One By One
    for (const BlogPost post : Posts) {
        // Check Post Name
        if (post.title == post_name) {
            // Erases Post at position
            Posts.erase(Posts.begin + i);
        }
        i++;
    }
}

// Constructor for Blog Post
BlogPost::BlogPost(char* title, char* description, char* creator) {
    this->title = title;
    this->description = description;
    this->creator = creator;
}
