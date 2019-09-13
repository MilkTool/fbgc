#ifndef FBGC_STL_H
#define FBGC_STL_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_stl_module;

//static struct fbgc_object * fbgc_print(struct fbgc_object * sm);
//extern const struct fbgc_cfunction fbgc_print_struct;

declare_new_fbgc_cfunction(fbgc_stl_initializer);
declare_new_fbgc_cfunction(fbgc_len);
declare_new_fbgc_cfunction(fbgc_matrix);



#ifdef  __cplusplus
}
#endif

#endif