#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "string.h"
#include "random.h"

int input_size;
int threadNumber;
int OFFSET;
int nx = 300;
int ny = 160;
int ns;
std::string temp[160][300];
vec3 lookfrom(13, 2, 3);
vec3 lookat(0, 0, 0);
float dist_to_focus = 10.0;
float aperture = 0.1;

camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);

vec3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable *random_scene()
{
    int n = 500;
    hitable **list = new hitable *[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                { // diffuse
                    list[i++] = new sphere(
                        center, 0.2,
                        new lambertian(vec3(random_double() * random_double(),
                                            random_double() * random_double(),
                                            random_double() * random_double())));
                }
                else if (choose_mat < 0.95)
                { // metal
                    list[i++] = new sphere(
                        center, 0.2,
                        new metal(vec3(0.5 * (1 + random_double()),
                                       0.5 * (1 + random_double()),
                                       0.5 * (1 + random_double())),
                                  0.5 * random_double()));
                }
                else
                { // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i);
}

hitable *world = random_scene();

void *thread_ray_trace(void *arg)
{
    int id = *(int *)arg;

    int start = id * (ny / threadNumber);
    int end = (id + 1) * (ny / threadNumber);
    if (id == threadNumber - 1)
        end += ny % threadNumber;
    for (int j = start; j < end; j++)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            temp[j][i] = std::to_string(ir) + " " + std::to_string(ig) + " " + std::to_string(ib) + "\n";
            //std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("invalid input\n");
        printf("./<programname> <number of threads> <size of pixel> \n");
        exit(1);
    }

    int input_size = atoi(argv[2]);
    threadNumber = atoi(argv[1]);

    struct timeval start, end;
    double time_spent;
    gettimeofday(&start, NULL);

    ns = input_size;
    //########### print picture ############
    // std::cout << "P3\n"
    //           << nx << " " << ny << "\n255\n";
    pthread_t threads[threadNumber];

    for (int i = 0; i < threadNumber; i++)
    {
        int *_th_id = (int *)calloc(1, sizeof(int));
        *_th_id = i;
        int rc = pthread_create(&threads[i], NULL, thread_ray_trace, (void *)_th_id);
        if (rc != 0)
        {
            printf("Error occurred, thread could not be created, errno = %d\n", rc);
            exit(0);
        }
    }
    for (int i = 0; i < threadNumber; i++)
    {
        pthread_join(threads[i], NULL);
    }
    //######## print picture #######
    // for (int j = ny - 1; j >= 0; j--)
    // {
    //     for (int i = 0; i < nx; i++)
    //     {
    //         std::cout << temp[j][i];
    //     }
    // }
    gettimeofday(&end, NULL);
    time_spent = ((double)((double)(end.tv_usec - start.tv_usec) / 1000000 +
                           (double)(end.tv_sec - start.tv_sec)));
    printf("Time taken for execution: %f seconds\n", time_spent);
}
