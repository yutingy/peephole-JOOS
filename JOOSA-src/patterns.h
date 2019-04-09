/*
 * JOOS is Copyright (C) 1997 Laurie Hendren & Michael I. Schwartzbach
 *
 * Reproduction of all or part of this software is permitted for
 * educational or research use on condition that this copyright notice is
 * included in any copy. This software comes with no warranty of any
 * kind. In no event will the authors be liable for any damages resulting from
 * use of this software.
 *
 * email: hendren@cs.mcgill.ca, mis@brics.dk
 */

/* iload x        iload x        iload x
 * ldc 0          ldc 1          ldc 2
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload x        iload x
 *                               dup
 *                               iadd
 */
#include <stdio.h>

int simplify_multiplication_right(CODE **c)
{ int x,k;
  if (is_iload(*c,&x) && 
      is_ldc_int(next(*c),&k) && 
      is_imul(next(next(*c)))) {
     if (k==0) return replace(c,3,makeCODEldc_int(0,NULL));
     else if (k==1) return replace(c,3,makeCODEiload(x,NULL));
     else if (k==2) return replace(c,3,makeCODEiload(x,
                                       makeCODEdup(
                                       makeCODEiadd(NULL))));
     return 0;
  }
  return 0;
}

/* dup
 * astore x
 * pop
 * -------->
 * astore x
 */
int simplify_astore(CODE **c)
{ int x;
  if (is_dup(*c) &&
      is_astore(next(*c),&x) &&
      is_pop(next(next(*c)))) {
     return replace(c,3,makeCODEastore(x,NULL));
  }
  return 0;
}

/* iload x
 * ldc k   (0<=k<=127)
 * iadd
 * istore x
 * --------->
 * iinc x k
 */ 
int positive_increment(CODE **c)
{ int x,y,k;
  if (is_iload(*c,&x) &&
      is_ldc_int(next(*c),&k) &&
      is_iadd(next(next(*c))) &&
      is_istore(next(next(next(*c))),&y) &&
      x==y && 0<=k && k<=127) {
     return replace(c,4,makeCODEiinc(x,k,NULL));
  }
  return 0;
}

/* goto L1
 * ...
 * L1:
 * goto L2
 * ...
 * L2:
 * --------->
 * goto L2
 * ...
 * L1:    (reference count reduced by 1)
 * goto L2
 * ...
 * L2:    (reference count increased by 1)  
 */
int simplify_goto_goto(CODE **c)
{ int l1,l2;
  if (is_goto(*c,&l1) && is_goto(next(destination(l1)),&l2) && l1>l2) {
     droplabel(l1);
     copylabel(l2);
     return replace(c,1,makeCODEgoto(l2,NULL));
  }
  return 0;
}

/*********************************************************
 * 
 * 
 * Our patterns
 * 
 * Group 06
 * Yu Ting Gu
 * Su Yu
 * Simon Zheng
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * ********************************************************/


/*
dup
istore k
pop
->
istore k
*/

int simplify_istore(CODE **c){
  int x;
  if(is_dup(*c)
  && is_istore(next(*c), &x)
  && is_pop(next(next(*c)))){
    return replace(c, 3, makeCODEistore(x, NULL));
  }

  return 0;
}

/* ldc 0          ldc 1          ldc 2 
 * iload x        iload x        iload x
 * imul           imul           imul
 * ------>        ------>        ------>
 * ldc 0          iload x        iload x
 *                               dup
 *                               iadd
 */

int simplify_multiplication_left(CODE **c)
{ int x,k;
  if (is_ldc_int(*c,&k) && 
      is_iload(next(*c),&x) && 
      is_imul(next(next(*c)))) {
     if (k==0) return replace(c,3,makeCODEldc_int(0,NULL));
     else if (k==1) return replace(c,3,makeCODEiload(x,NULL));
     else if (k==2) return replace(c,3,makeCODEiload(x,
                                       makeCODEdup(
                                       makeCODEiadd(NULL))));
     return 0;
  }
  return 0;
}

/*
iload x
ldc 1
idiv
->
iload x
*/
int simplify_division_right(CODE **c)
{ int x,k;
  if (is_iload(*c,&x) && 
      is_ldc_int(next(*c),&k) && 
      is_idiv(next(next(*c)))) {
     if (k==1) return replace(c,3,makeCODEiload(x,NULL));
  }
  return 0;
}


/*
iinc x 0        iload x         ldc_int 0
->              ldc_int 0       iload x
(nothing)       iadd            iadd
                ->              ->
                iload x         iload x
*/
int inc0(CODE **c){
  int offset, amount;
  if(is_iinc(*c, &offset, &amount)){
    if(amount==0){
      return replace(c, 1, NULL);
    }
  }

  int x, k;
  if(is_iload(*c, &x) 
  && is_ldc_int(next(*c), &k) && k==0 
  && is_iadd(next(next(*c)))){
    return replace(c,3, makeCODEiload(x, NULL));
  } 
  if(is_ldc_int(*c, &k) && k==0 
  && is_iload(next(*c), &x) 
  && is_iadd(next(next(*c)))){
    return replace(c,3, makeCODEiload(x, NULL));
  }
  return 0;
}

/*
iload x
ldc_int 0    
isub
-> 
iload x

*/
int sub_by_zero(CODE **c){
  int x,k;
  if(is_iload(*c, &x) 
  && is_ldc_int(next(*c), &k) 
  && (is_isub(next(next(*c))))
  && k==0){
    return replace(c, 2, makeCODEiload(x, NULL));
  }
  return 0;
}

/*
ldc_int 0
iload x
isub
->
iload x
ineg
*/

int negate(CODE **c){

  int x,k;
  if(is_ldc_int(*c, &k)
    && k==0
    && is_iload(next(*c), &x)
    && is_isub(next(next(*c)))){
      return replace(c, 3, makeCODEiload(x, makeCODEineg(NULL)));
    }

  return 0;


}

/*
aconst_null     ldc_string k
ifnull L        ifnull L
->              ->
goto L          (remove those two lines)


aconst_null                 ldc_string k       
ifnonnull L                 ifnonnull L   
->                          ->            
(remove those two lines)    goto L

*/

int load_branch_null(CODE **c){

  int label;

  if(is_aconst_null(*c)
  && is_ifnull(next(*c), &label)){
    return replace(c,2, makeCODEgoto(label,NULL));
  }

  char *str;
  if(is_ldc_string(*c, &str)
  && is_ifnull(next(*c), &label)){
    return replace(c, 2, NULL);
  }

  if(is_aconst_null(*c)
  && is_ifnonnull(next(*c), &label)){
    return replace(c, 2, NULL);
  }

  if(is_ldc_string(*c, &str)
  && is_ifnonnull(next(*c), &label)){
    return replace(c, 2, makeCODEgoto(label, NULL));
  }

  return 0;

}

/*

iload x       iload x       ldc_int k
iload y       ldc_int k     iload x
swap          swap          swap
->            ->            ->
iload y       ldc_int k     iload x
iload x       iload x       ldc_int k

same with aload:

aload x       aload x       ldc_string k
aload y       ldc_string k  aload x
swap          swap          swap
->            ->            ->
aload y       ldc_string k  aload x
aload x       aload x       ldc_string k

similar case with getfield:

aload x       aload y       aload x
aload y       getfield f    dup
getfield f    aload x       getfield f
swap          swap          swap
->            ->            ->
aload y       aload x       aload x
getfield f    aload y       getfield f
aload x       getfield f    aload x

and a mix...

iconst x      aload x
aload y       iconst y
swap          swap
->            ->
aload y       iconst y
iconst x      aload x

Seen in bench03-conversion:

new ..
dup
invokenonvirtual
aload x
swap
->
aload x
new ..
dup
invoke

*/
int swap(CODE **c){
  int x,y,k;
  char* str;
  char* args;
  if(is_iload(*c, &x)
  && is_iload(next(*c), &y)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEiload(y, makeCODEiload(x, NULL)));
  }

  if(is_iload(*c, &x)
  && is_ldc_int(next(*c), &k)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEldc_int(k, makeCODEiload(x, NULL)));
  }

  if(is_ldc_int(*c, &k)
  && is_iload(next(*c), &x)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEiload(x, makeCODEldc_int(k, NULL)));
  }


  if(is_aload(*c, &x)
  && is_aload(next(*c), &y)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEaload(y, makeCODEaload(x, NULL)));
  }

  if(is_aload(*c, &x)
  && is_ldc_string(next(*c), &str)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEldc_string(str, makeCODEaload(x, NULL)));
  }

  if(is_ldc_string(*c, &str)
  && is_aload(next(*c), &x)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEaload(x, makeCODEldc_string(str, NULL)));
  }

  if(is_aload(*c, &x)
  && is_aload(next(*c), &y)
  && is_getfield(next(next(*c)), &str)
  && is_swap(next(next(next(*c))))){
    return replace(c, 4, makeCODEaload(y, makeCODEgetfield(str, makeCODEaload(x, NULL))));
  }

  if(is_aload(*c, &y)
  && is_getfield(next(*c), &str)
  && is_aload(next(next(*c)), &x)
  && is_swap(next(next(next(*c))))){
    return replace(c, 4, makeCODEaload(x, makeCODEaload(y, makeCODEgetfield(str, NULL))));
  }

  if(is_aload(*c, &x)
  && is_dup(next(*c))
  && is_getfield(next(next(*c)), &str)
  && is_swap(next(next(next(*c))))){
    return replace(c, 4, makeCODEaload(x, makeCODEgetfield(str, makeCODEaload(x, NULL))));
  }

  if(is_ldc_int(*c, &x)
  && is_aload(next(*c), &y)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEaload(y, makeCODEldc_int(x, NULL)));
  }

  if(is_aload(*c, &y)
  && is_ldc_int(next(*c), &x)
  && is_swap(next(next(*c)))){
    return replace(c, 3, makeCODEldc_int(x, makeCODEaload(y, NULL)));
  }

  if(is_new(*c, &str)
  && is_dup(next(*c))
  && is_invokenonvirtual(next(next(*c)), &args)
  && is_aload(next(next(next(*c))), &x)
  && is_swap(next(next(next(next(*c)))))){
    return replace(c, 5, makeCODEaload(x, makeCODEnew(str, makeCODEdup(makeCODEinvokenonvirtual(args, NULL)))));
  }

  return 0;
}

/*

dup                                               dup                     
if_icmpeq label (or if_icmpge, if_icmple)         if_icmpne label      (or if_icmpgt, if_icmplt)
->                                                ->
pop                                               pop (drop count on label)
goto label

first case does not actually reduce code size 
and same with if_acmpeq and if_acmpne 
*/

int dup_stack_compares(CODE **c){

  int label;

  if(is_dup(*c)){

    /* first, icmpeq and icmpne */
    
    if(is_if_icmpeq(next(*c), &label) || is_if_icmpge(next(*c), &label) || is_if_icmple(next(*c), &label)){
      return replace(c, 2, makeCODEpop(makeCODEgoto(label, NULL)));
    }
    if(is_if_icmpne(next(*c), &label) || is_if_icmpgt(next(*c), &label) || is_if_icmplt(next(*c), &label)){
      droplabel(label);
      return replace(c,2,makeCODEpop(NULL));
    }

    /* same thing, with acmpeq and acmpne */

    if(is_if_acmpeq(next(*c), &label)){
      return replace(c, 2, makeCODEpop(makeCODEgoto(label, NULL)));
    } 
    if(is_if_acmpne(next(*c), &label)){
      droplabel(label);
      return replace(c,2,makeCODEpop(NULL));
    }
  }

  return 0;
}


/*
goto label
... (any non-label line)
->
goto label
*/
int after_goto_unreachable(CODE **c){

  int label, somecode;
  if(is_goto(*c, &label)
  && !is_label(next(*c), &somecode)){
    return replace_modified(c, 2, makeCODEgoto(label,NULL));
  }

  return 0;

}


/* iload x
 * ldc k   (0<=k<=127)
 * isub
 * istore x
 * --------->
 * iinc x -k
 * 
 * dup
 * istore x
 * ldc k (0<=k<=127)
 * isub
 * istore x
 * ->
 * iinc x -k
 * 
 */ 
int decrement(CODE **c)
{ int x,y,k;
  if (is_iload(*c,&x) &&
      is_ldc_int(next(*c),&k) &&
      is_isub(next(next(*c))) &&
      is_istore(next(next(next(*c))),&y) &&
      x==y && 0<=k && k<=127) {
     return replace(c,4,makeCODEiinc(x,-k,NULL));
  }

  if(is_dup(*c)
  && is_istore(next(*c), &x)
  && is_ldc_int(next(next(*c)), &k)
  && is_isub(next(next(next(*c))))
  && is_istore(next(next(next(next(*c)))), &y)
  && x==y && 0<=k && k<=127){
    return replace(c,5,makeCODEiinc(x, -k, NULL));
  }
  return 0;
}


/*
ldc_int 0
if_cmpeq label
->
ifeq label

ldc_int 0
if_cmpne label
->
ifne label

*/
int icmp_zero(CODE **c){
  
  int k, label;
  
  if(is_ldc_int(*c, &k)
  && is_if_icmpeq(next(*c), &label)
  && k==0){
    return replace(c, 2, makeCODEifeq(label, NULL));
  }

  if(is_ldc_int(*c, &k)
  && is_if_icmpne(next(*c), &label)
  && k==0){
    return replace(c, 2, makeCODEifne(label, NULL));
  } 

  return 0;
}

/* bench03 decoder.j

behavior: condition -> not L3, else -> L3

if_condition label1
iconst_0
goto label2
label1:
iconst_1
label2:
ifeq label3
->
if_opposite-condition label3

label1 and label2 drop reference counts by 1 each
and this pattern is reduced iff label1 and label2 are dead after the reference count drops
*/
int simplify_if_gotos(CODE **c){
  int label1, label2, label3, label4, label5, x, y;

  if(is_ifeq(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEifne(label5, NULL));
    }

  }

  if(is_ifne(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEifeq(label5, NULL));
    }
  }

  if(is_if_icmpne(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_icmpeq(label5, NULL));
    }
  }

  if(is_if_icmpeq(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_icmpne(label5, NULL));
    }
  }

  if(is_ifnonnull(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEifnull(label5, NULL));
    }
  }

  if(is_if_icmpge(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_icmplt(label5, NULL));
    }
  }

  if(is_if_icmplt(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_icmpge(label5, NULL));
    }
  }

  if(is_if_icmple(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_icmpgt(label5, NULL));
    }
  }

  if(is_if_icmpgt(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_icmple(label5, NULL));
    }
  }

  if(is_ifnull(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEifnonnull(label5, NULL));
    }
  }

  if(is_if_acmpeq(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_acmpne(label5, NULL));
    }
  }

  if(is_if_acmpne(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_ifeq(nextby(*c, 6), &label5)){
    if(uniquelabel(label1)&&uniquelabel(label2)){
      droplabel(label1);
      droplabel(label2);
      return replace(c, 7, makeCODEif_acmpeq(label5, NULL));
    }
  }

  return 0;

}

/* bench03 decoder.j. same as above but with two lines of code in the middle

behavior: condition -> stack has 1 on top, reached after-pop stmt
           else -> stack is unchanged, and we are at label3

          
          ifcondition newlabel  (reuse label1 here as a newlabel)
          goto label3
          newlabel:
          iconst 1          
          ==
          elsecondition label3
          iconst 1   
               

if_condition label1
iconst_0
goto label2
label1:
iconst_1
label2:
dup
ifeq label3
pop
->
elsecondition label3
iconst 1   

label1 label2 removed iff they are unique & this pattern can be reduced
*/

int simplify_if_gotos2(CODE **c){
  int label1, label2, label3, label4, label5, x, y;

  if(is_ifeq(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEifne(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_ifne(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEifeq(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_icmpne(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_icmpeq(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_icmpeq(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_icmpne(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_ifnonnull(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEifnull(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_icmpge(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_icmplt(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_icmplt(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
    && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_icmpge(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_icmple(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){
    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_icmpgt(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_icmpgt(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_icmple(label5, makeCODEldc_int(1, NULL)));
  } 

  if(is_ifnull(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEifnonnull(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_acmpeq(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_acmpne(label5, makeCODEldc_int(1, NULL)));
  }

  if(is_if_acmpne(*c, &label1)
  && is_ldc_int(next(*c), &x)
  && x==0
  && is_goto(nextby(*c, 2), &label2)
  && is_label(nextby(*c, 3), &label3)
  && label1==label3
  && is_ldc_int(nextby(*c, 4), &y)
  && y==1
  && is_label(nextby(*c, 5), &label4)
  && label2==label4
  && is_dup(nextby(*c, 6))
  && is_ifeq(nextby(*c, 7), &label5)
  && is_pop(nextby(*c, 8))
  && uniquelabel(label1) && uniquelabel(label2)){

    droplabel(label1);
    droplabel(label2);

    return replace(c, 9, makeCODEif_acmpeq(label5, makeCODEldc_int(1, NULL)));
  }

  return 0;

}

/*
ldc_int 0
iload x
if_icmpeq label
->
iload x
ifeq label

ldc_int 0
iload x
if_icmpne label
->
iload x
ifne label
*/
int simplify_load_zero_icmp(CODE **c){
  int k,x,label;
  if(is_ldc_int(*c, &k)
  && is_iload(next(*c), &x)
  && k==0
  && is_if_icmpeq(next(next(*c)), &label)){
    return replace(c, 3, makeCODEiload(x, makeCODEifeq(label, NULL)));
  }

  if(is_ldc_int(*c, &k)
  && is_iload(next(*c), &x)
  && k==0
  && is_if_icmpne(next(next(*c)), &label)){
    return replace(c, 3, makeCODEiload(x, makeCODEifne(label, NULL)));
  }  
  
  return 0;

}







/*

goto label1 
...
label1:
label2:
->
goto label2:
...
label1: (ref count dropped by 1)
label2: (ref count increased by 1) 

same with all other ways of branching: ifeq, ifne, if_icmpeq, if_icmpne,if_icmpge, if_icmpgt, if_icmple, if_icmpge, ifnull, ifnonnull

this will eventually remove the label label1
*/
int simplify_double_label(CODE **c){
  int label1, label2;
  if(is_goto(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEgoto(label2, NULL));
  }

  if(is_ifeq(*c, &label1)
  && is_label(next(destination(label1)), &label2)&& label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifeq(label2, NULL));
  }

  if(is_ifne(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifne(label2, NULL));
  }

  if(is_if_icmpeq(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpeq(label2, NULL));
  }

  if(is_if_icmpne(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpeq(label2, NULL));
  }

  if(is_if_icmpge(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpge(label2, NULL));
  }

  if(is_if_icmpgt(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpgt(label2, NULL));
  }

  if(is_if_icmplt(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmplt(label2, NULL));
  }

  if(is_if_icmple(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmple(label2, NULL));
  }

  if(is_ifnonnull(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifnonnull(label2, NULL));
  }

  if(is_ifnull(*c, &label1)
  && is_label(next(destination(label1)), &label2) && label1 > label2){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifnull(label2, NULL));
  }

  return 0;

}


/*
goto label
...
label:
return/areturn/ireturn
->
return/areturn/ireturn
...
label: (reference count decreased)
return/areturn/ireturn

*/
int goto_return(CODE **c){
  int label;
  if((is_goto(*c, &label))
  && is_return(next(destination(label)))){
    droplabel(label);
    return replace(c, 1, makeCODEreturn(NULL));
  }
  if((is_goto(*c, &label))
  && is_areturn(next(destination(label)))){
    droplabel(label);
    return replace(c, 1, makeCODEareturn(NULL));
  }
  if((is_goto(*c, &label))
  && is_ireturn(next(destination(label)))){
    droplabel(label);
    return replace(c, 1, makeCODEireturn(NULL));
  }

  return 0;
}


/*
return/areturn/ireturn
(non-label line) (and not a nop)
->
return/areturn/ireturn
*/
int after_return_unreachable(CODE **c){
  int label;
  if(is_return(*c)
  && next(*c)!=NULL
  && !is_label(next(*c), &label)){
    return replace_modified(c, 2, makeCODEreturn(NULL));
  }
  if(is_areturn(*c)
  && next(*c)!=NULL
  && !is_label(next(*c), &label)){
    return replace_modified(c, 2, makeCODEareturn(NULL));
  }
  if(is_ireturn(*c)  
  && next(*c)!=NULL
  && !is_label(next(*c), &label)){
    return replace_modified(c, 2, makeCODEireturn(NULL));
  }

  return 0;
}




/*
ifeq label1
goto label2
label1:
->
ifne label2
label1: (ref count dropped)

and similar for other opposing branching (gt-le, ge-lt, null-nonnull, acmpeq-acmpne),
*/
int simplify_ifs_gotos(CODE **c){

  int label1, label2, label3;

  if(is_ifeq(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEifne(label2, NULL));
  }

  if(is_ifne(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEifeq(label2, NULL));
  }

  if(is_if_icmpeq(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_icmpne(label2, NULL));
  }

  if(is_if_icmpne(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_icmpeq(label2, NULL));
  }

  if(is_if_acmpeq(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_acmpne(label2, NULL));
  }

  if(is_if_acmpne(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_acmpeq(label2, NULL));
  }

  if(is_if_icmpgt(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_icmple(label2, NULL));
  }

  if(is_if_icmple(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_icmpgt(label2, NULL));
  }

  if(is_if_icmplt(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_icmpge(label2, NULL));
  }

  if(is_if_icmpge(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_icmplt(label2, NULL));
  }

  if(is_ifnull(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEifnonnull(label2, NULL));
  }

  if(is_if_acmpeq(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_acmpne(label2, NULL));
  }

  if(is_if_acmpne(*c, &label1)
  && is_goto(next(*c), &label2)
  && is_label(next(next(*c)),&label3)
  && label1==label3){
    droplabel(label1);
    return replace(c, 2, makeCODEif_acmpeq(label2, NULL));
  }

  return 0;

}


/*
goto label1
...
label1:
ifcondition label2
...
label2
->
ifcondition label2
...
label1: (dropped reference)
ifcondition label2
...
label2 (added reference)
*/
int goto_another_conditional(CODE **c){
  int label1, label2;

  if(is_goto(*c, &label1)
  && is_ifeq(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifeq(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_ifne(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifne(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_icmpeq(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpeq(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_icmpne(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpne(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_icmpge(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpge(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_icmple(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmple(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_icmpgt(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmpgt(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_icmplt(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_icmplt(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_ifnonnull(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifnonnull(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_ifnull(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEifnull(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_acmpeq(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_acmpeq(label2, NULL));
  }

  if((is_goto(*c, &label1))
  && is_if_acmpne(next(destination(label1)), &label2)){
    droplabel(label1);
    copylabel(label2);
    return replace(c, 1, makeCODEif_acmpne(label2, NULL));
  }

  return 0;

}

/*
astore x        istore x
aload x         iload x
->              ->
dup             dup
astore x        istore x
*/
int store_load(CODE **c){
  int x,y;
  if(is_astore(*c, &x)
  && is_aload(next(*c), &y)
  && x==y){
    return replace(c, 2, makeCODEdup(makeCODEastore(x, NULL)));
  }

  if(is_istore(*c, &x)
  && is_iload(next(*c), &y)
  && x==y){
    return replace(c, 2, makeCODEdup(makeCODEistore(x, NULL)));
  }

  return 0;

}



/*Interpretor.j bench02

dup
aload k
swap
putfield f sig
pop
->
aload k
swap
putfield f sig
*/
int putfield_dup_pop(CODE **c){
  int k;
  char* arg;
  if(is_dup(*c)
  && is_aload(next(*c), &k)
  && is_swap(next(next(*c)))
  && is_putfield(next(next(next(*c))), &arg)
  && is_pop(next(next(next(next(*c)))))){
    return replace(c, 5, makeCODEaload(k, makeCODEswap(makeCODEputfield(arg, NULL))));
  }
  return 0;
}

/*
aconst_null
if_acmpeq label
->
ifnull label 

aconst_null
if_acmpne label
->
ifnonnull label
*/
int loadnull_acmp(CODE **c){
  int label;

  if(is_aconst_null(*c)
  && is_if_acmpeq(next(*c), &label)){
    return replace(c, 2, makeCODEifnull(label, NULL));
  }

  if(is_aconst_null(*c)
  && is_if_acmpne(next(*c), &label)){
    return replace(c, 2, makeCODEifnonnull(label, NULL));
  }
  
  return 0;
}



/* seen in bench04 
iload x
iconst y (0<=y<=127)
isub
istore k
->
iload x
istore k
iinc k -y
*/
int stack_arithmetic_store(CODE **c){
  int x,y,k;

  if(is_iload(*c, &x)
  && is_ldc_int(next(*c), &y)
  && 0<=y && y<=127
  && is_isub(next(next(*c)))
  && is_istore(next(next(next(*c))), &k)
  && x!=k){
    return replace(c, 4, makeCODEiload(x, makeCODEistore(k, makeCODEiinc(k, -y, NULL))));
  }

  if(is_iload(*c, &x)
  && is_ldc_int(next(*c), &y)
  && 0<=y && y<=127
  && is_isub(next(next(*c)))
  && is_istore(next(next(next(*c))), &k)
  && x!=k){
    return replace(c, 4, makeCODEiload(x, makeCODEistore(k, makeCODEiinc(k, -y, NULL))));
  }

  return 0;

}

/*

label: [dead]
nop
.endmethod
->
.endmethod

*/
int purge_nop_label(CODE **c){
  int label;
  if(is_label(*c, &label)
  && is_nop(next(*c))
  && deadlabel(label)){
    return replace(c, 2, NULL);
  }
  return 0;
}


/*
label1:
goto label2:
...
label2: [unique]
nop
.endmethod
->
label1:
nop
...
label2: [dead]
nop
.endmethod

this will be further reduced with purge_nop_label function above
*/
int goto_goto_nop_end(CODE **c){
  int label1, label2;
  if(is_label(*c, &label1)
  && is_goto(next(*c), &label2)
  && uniquelabel(label2)
  && is_nop(next(destination(label2)))
  && next(next(destination(label2)))==NULL){
    droplabel(label2);
    return replace(c, 2, makeCODElabel(label1, makeCODEnop(NULL)));
  }
  return 0;
}

/*
label: [dead]
->

*/
int remove_dead_labels(CODE **c){
  int label;

  if(is_label(*c, &label) && deadlabel(label)){
    return replace(c, 1, NULL);
  }

  return 0;
}


/* bench05 treasureroomaction.j

condition true -> [1] -> [1 1] -> [1] at label3
          false -> [0]-> [0 0] -> [0] -> [] (popped at pop line)

if_icmpge label0
iconst 0
goto label1
label0: [unique]
iconst 1
label1: [unique]
dup
ifne label3
pop
...
label3:
->
if_icmplt label1
iconst 1 
goto label3
label1:
...
label3:
*/

int simplify_bench05_branching(CODE **c){
  int x, y, label0, label1, label2, label3, label4;

  if(is_if_icmpge(*c, &label0)
  && is_ldc_int(next(*c), &x)
  && is_goto(nextby(*c, 2), &label1)
  && is_label(nextby(*c,3), &label2) && label0==label2
  && is_ldc_int(nextby(*c, 4), &y)
  && x==0 && y==1
  && is_label(nextby(*c, 5), &label3) 
  && label1==label3 && uniquelabel(label1) && uniquelabel(label0)
  && is_dup(nextby(*c, 6))
  && is_ifne(nextby(*c,7), &label4)
  && is_pop(nextby(*c, 8))){
    droplabel(label4);
    return replace(c, 9, makeCODEif_icmplt(label1, makeCODEldc_int(1, makeCODEgoto(label4, makeCODElabel(label1, NULL)))));
  }

  return 0;

}



void init_patterns(void) {
	ADD_PATTERN(simplify_multiplication_right);
	ADD_PATTERN(simplify_astore);
	ADD_PATTERN(positive_increment);
	ADD_PATTERN(simplify_goto_goto);

  

  ADD_PATTERN(simplify_istore);
  ADD_PATTERN(simplify_multiplication_left);
  ADD_PATTERN(simplify_division_right);
  ADD_PATTERN(inc0);
  ADD_PATTERN(decrement);
  ADD_PATTERN(dup_stack_compares);
  ADD_PATTERN(sub_by_zero);
  ADD_PATTERN(negate);
  ADD_PATTERN(load_branch_null);
  ADD_PATTERN(swap);
  ADD_PATTERN(icmp_zero);
  ADD_PATTERN(after_goto_unreachable);
  ADD_PATTERN(after_return_unreachable);
  ADD_PATTERN(goto_return);
  ADD_PATTERN(goto_another_conditional);
  ADD_PATTERN(store_load);
  ADD_PATTERN(simplify_ifs_gotos);
  ADD_PATTERN(simplify_load_zero_icmp);
  ADD_PATTERN(putfield_dup_pop);
  ADD_PATTERN(purge_nop_label);
  ADD_PATTERN(goto_goto_nop_end);
  ADD_PATTERN(simplify_double_label);
  ADD_PATTERN(simplify_if_gotos);
  ADD_PATTERN(loadnull_acmp);
  ADD_PATTERN(stack_arithmetic_store);
  ADD_PATTERN(simplify_bench05_branching);

  
  /*


    ADD_PATTERN(remove_dead_labels);



    






  */
}
