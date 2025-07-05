#include <3ds.h>

#include "..\include\includes.h"

int parseJson(char *jsonPath) {
    json_error_t error;
    json_t *root = json_loads(readFile(CACHE_JSON), 0, &error);

    if(!root) {
        fprintf(stderr, "Error Parsing json, Exiting...");
        return 1;
    }

    json_t *posts = json_object_get(root, "posts");
    if(!json_is_array(posts)) {
        fprintf(stderr, "Invalid Json");
        return 1;
    }

    size_t index;
    json_t *post;

    json_array_foreach(posts, index, post) {
        json_t *id = json_object_get(post, "id");
        if(json_is_integer(id)) {
            printf("Post ID: %" JSON_INTEGER_FORMAT "\n", json_integer_value(id));
        }

        const char *created_at = json_string_value(json_object_get(post, "created_at"));
        const char *updated_at = json_string_value(json_object_get(post, "updated_at"));
        printf("Created At: %s\n", created_at ? created_at : "N/A");
        printf("Updated At: %s\n", updated_at ? updated_at : "N/A");

        const char *file_url = NULL;
        json_t *file = json_object_get(post, "file");
        if(json_is_object(file)) {
            json_t *url = json_object_get(file, "url");
            if(json_is_string(url)) {
                file_url = json_string_value(url);
                printf("File URL: %s\n", file_url);
            }
        }

        const char *tags_get = NULL;
        json_t *tags = json_object_get(post, "tags");
        if(json_is_object(tags)) {
            json_t *general = json_object_get(tags, "general");
            if(json_is_array(general)) {
                tags_get = json_
            }
        }
    }
}