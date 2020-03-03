#!/bin/sh                                                                                                                                                                                                   
pushd resource                                                  
tar czf hidoo_third_soft.tgz third_soft/                              
tar czf nova.tgz nova/                                          
popd                                                            
                                                                
#pushd project                                                   
#tar czf sysroot.tgz sysroot                                     
#popd
