.class public HuntRoomAction

.super RoomAction

.field protected taken Z
.field protected target Ljava/lang/String;

.method public <init>()V
  .limit locals 2
  .limit stack 2
  aload_0
  invokenonvirtual RoomAction/<init>()V
  aload_0
  iconst_4
  invokevirtual RoomAction/rand(I)I
  dup
  istore_1
  ifne else_0
  aload_0
  ldc "fierce ogre"
  putfield HuntRoomAction/target Ljava/lang/String;
  goto stop_1
  else_0:
  iload_1
  iconst_1
  if_icmpne else_4
  aload_0
  ldc "rabbit"
  putfield HuntRoomAction/target Ljava/lang/String;
  goto stop_5
  else_4:
  iload_1
  iconst_2
  if_icmpne else_8
  aload_0
  ldc "dark demon"
  putfield HuntRoomAction/target Ljava/lang/String;
  goto stop_9
  else_8:
  iload_1
  iconst_3
  if_icmpne else_12
  aload_0
  ldc "puppy"
  putfield HuntRoomAction/target Ljava/lang/String;
  goto stop_13
  else_12:
  aload_0
  ldc "wtf"
  putfield HuntRoomAction/target Ljava/lang/String;
  stop_13:
  stop_9:
  stop_5:
  stop_1:
  aload_0
  iconst_0
  putfield HuntRoomAction/taken Z
  return
.end method

.method public describe()V
  .limit locals 1
  .limit stack 4
  aload_0
  getfield HuntRoomAction/taken Z
  ifne else_0
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "A wild "
  goto stop_7
  null_6:
  pop
  ldc "null"
  stop_7:
  aload_0
  getfield HuntRoomAction/target Ljava/lang/String;
  dup
  ifnonnull stop_9
  null_8:
  pop
  ldc "null"
  stop_9:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  dup
  ifnonnull stop_5
  null_4:
  pop
  ldc "null"
  stop_5:
  ldc " appears!"
  goto stop_11
  null_10:
  pop
  ldc "null"
  stop_11:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  return
  else_0:
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "The carcass of a "
  goto stop_15
  null_14:
  pop
  ldc "null"
  stop_15:
  aload_0
  getfield HuntRoomAction/target Ljava/lang/String;
  dup
  ifnonnull stop_17
  null_16:
  pop
  ldc "null"
  stop_17:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  dup
  ifnonnull stop_13
  null_12:
  pop
  ldc "null"
  stop_13:
  ldc " lies before you."
  goto stop_19
  null_18:
  pop
  ldc "null"
  stop_19:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  stop_1:
  return
.end method

.method public performAction(Ljava/lang/String;)I
  .limit locals 2
  .limit stack 4
  aload_1
  ldc "strike"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_15
  iconst_0
  goto stop_16
  true_15:
  iconst_1
  stop_16:
  dup
  ifne true_14
  pop
  aload_1
  ldc "attack"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_17
  iconst_0
  goto stop_18
  true_17:
  iconst_1
  stop_18:
  true_14:
  dup
  ifne true_13
  pop
  aload_1
  ldc "hunt"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_19
  iconst_0
  goto stop_20
  true_19:
  iconst_1
  stop_20:
  true_13:
  dup
  ifne true_12
  pop
  aload_1
  ldc "interact"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_21
  iconst_0
  goto stop_22
  true_21:
  iconst_1
  stop_22:
  true_12:
  dup
  ifne true_11
  pop
  aload_1
  ldc "shoot"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_23
  iconst_0
  goto stop_24
  true_23:
  iconst_1
  stop_24:
  true_11:
  dup
  ifne true_10
  pop
  aload_1
  ldc "hit"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_25
  iconst_0
  goto stop_26
  true_25:
  iconst_1
  stop_26:
  true_10:
  dup
  ifne true_9
  pop
  aload_1
  ldc "ram"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_27
  iconst_0
  goto stop_28
  true_27:
  iconst_1
  stop_28:
  true_9:
  dup
  ifne true_8
  pop
  aload_1
  ldc "charge"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_29
  iconst_0
  goto stop_30
  true_29:
  iconst_1
  stop_30:
  true_8:
  dup
  ifne true_7
  pop
  aload_1
  ldc "punch"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_31
  iconst_0
  goto stop_32
  true_31:
  iconst_1
  stop_32:
  true_7:
  dup
  ifne true_6
  pop
  aload_1
  ldc "fight"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_33
  iconst_0
  goto stop_34
  true_33:
  iconst_1
  stop_34:
  true_6:
  dup
  ifne true_5
  pop
  aload_1
  ldc "kick"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_35
  iconst_0
  goto stop_36
  true_35:
  iconst_1
  stop_36:
  true_5:
  dup
  ifne true_4
  pop
  aload_1
  ldc "grab"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_37
  iconst_0
  goto stop_38
  true_37:
  iconst_1
  stop_38:
  true_4:
  dup
  ifne true_3
  pop
  aload_1
  ldc "swing"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_39
  iconst_0
  goto stop_40
  true_39:
  iconst_1
  stop_40:
  true_3:
  dup
  ifne true_2
  pop
  aload_1
  ldc "hurt"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_41
  iconst_0
  goto stop_42
  true_41:
  iconst_1
  stop_42:
  true_2:
  ifeq else_0
  aload_0
  getfield HuntRoomAction/taken Z
  ifeq else_43
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "The "
  goto stop_48
  null_47:
  pop
  ldc "null"
  stop_48:
  aload_0
  getfield HuntRoomAction/target Ljava/lang/String;
  dup
  ifnonnull stop_50
  null_49:
  pop
  ldc "null"
  stop_50:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  dup
  ifnonnull stop_46
  null_45:
  pop
  ldc "null"
  stop_46:
  ldc " is already dead."
  goto stop_52
  null_51:
  pop
  ldc "null"
  stop_52:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  iconst_0
  ireturn
  else_43:
  aload_0
  ldc 100
  invokevirtual RoomAction/rand(I)I
  ldc 60
  if_icmple else_53
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "Your attack succeeded!"
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "You rip out the heart of your prey and takes a bite."
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  aload_0
  getfield HuntRoomAction/target Ljava/lang/String;
  ldc "puppy"
  invokevirtual java/lang/String/equals(Ljava/lang/Object;)Z
  ifeq else_57
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "You monster!"
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  goto stop_58
  else_57:
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "You feel energized."
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  stop_58:
  aload_0
  iconst_1
  putfield HuntRoomAction/taken Z
  ldc 8
  ireturn
  else_53:
  aload_0
  ldc 100
  invokevirtual RoomAction/rand(I)I
  ldc 80
  if_icmple else_59
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "Your attack failed. But you managed to dodge the "
  goto stop_66
  null_65:
  pop
  ldc "null"
  stop_66:
  aload_0
  getfield HuntRoomAction/target Ljava/lang/String;
  dup
  ifnonnull stop_68
  null_67:
  pop
  ldc "null"
  stop_68:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  dup
  ifnonnull stop_64
  null_63:
  pop
  ldc "null"
  stop_64:
  ldc "'s attack and avoid taking damage."
  goto stop_70
  null_69:
  pop
  ldc "null"
  stop_70:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  iconst_0
  ireturn
  else_59:
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "Your attack failed. The counter attack from the "
  goto stop_74
  null_73:
  pop
  ldc "null"
  stop_74:
  aload_0
  getfield HuntRoomAction/target Ljava/lang/String;
  dup
  ifnonnull stop_76
  null_75:
  pop
  ldc "null"
  stop_76:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  dup
  ifnonnull stop_72
  null_71:
  pop
  ldc "null"
  stop_72:
  ldc " left you wounded."
  goto stop_78
  null_77:
  pop
  ldc "null"
  stop_78:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  ldc 10
  ireturn
  stop_60:
  goto stop_1
  else_0:
  aload_1
  ldc "run"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_83
  iconst_0
  goto stop_84
  true_83:
  iconst_1
  stop_84:
  dup
  ifne true_82
  pop
  aload_1
  ldc "escape"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_85
  iconst_0
  goto stop_86
  true_85:
  iconst_1
  stop_86:
  true_82:
  dup
  ifne true_81
  pop
  aload_1
  ldc "leave"
  iconst_0
  invokevirtual java/lang/String/indexOf(Ljava/lang/String;I)I
  iconst_0
  if_icmpge true_87
  iconst_0
  goto stop_88
  true_87:
  iconst_1
  stop_88:
  true_81:
  ifeq else_79
  aload_0
  ldc 100
  invokevirtual RoomAction/rand(I)I
  ldc 90
  if_icmpgt true_91
  iconst_0
  goto stop_92
  true_91:
  iconst_1
  stop_92:
  dup
  ifeq false_90
  pop
  aload_0
  getfield HuntRoomAction/taken Z
  ifeq true_93
  iconst_0
  goto stop_94
  true_93:
  iconst_1
  stop_94:
  false_90:
  ifeq stop_89
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "The "
  goto stop_98
  null_97:
  pop
  ldc "null"
  stop_98:
  aload_0
  getfield HuntRoomAction/target Ljava/lang/String;
  dup
  ifnonnull stop_100
  null_99:
  pop
  ldc "null"
  stop_100:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  dup
  ifnonnull stop_96
  null_95:
  pop
  ldc "null"
  stop_96:
  ldc " was startled by your indecision. It ran away."
  goto stop_102
  null_101:
  pop
  ldc "null"
  stop_102:
  invokevirtual java/lang/String/concat(Ljava/lang/String;)Ljava/lang/String;
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  aload_0
  iconst_1
  putfield HuntRoomAction/taken Z
  stop_89:
  aload_0
  getfield RoomAction/O Ljoos/lib/JoosIO;
  ldc "Which direction?"
  invokevirtual joos/lib/JoosIO/println(Ljava/lang/String;)V
  iconst_0
  ireturn
  else_79:
  aload_0
  aload_1
  invokevirtual RoomAction/performBaseAction(Ljava/lang/String;)I
  ireturn
  stop_80:
  stop_1:
  nop
.end method

