import numpy as np
import tensorflow as tf
import os

class Learner(object):
    def __init__(self,net_params=None):
        self.out_dim = net_params.get('out_dim',6)
        self.n_conv_layers = net_params.get('n_conv_layers',5)
        self.n_fc_layers = net_params.get('n_fc_layers',3 + 1)
        self.n_temp_layers = net_params.get('n_temp_layers',2)
        self.n_filters = net_params.get('n_filters',[64]*self.n_conv_layers)
        self.kernel_size = net_params.get('kernel_size',[3]*self.n_conv_layers)
        self.stride = net_params.get('stride',[2,2,2,1,1])
        self.hidden_dim =  net_params.get('hidden_dim'[100]*(self.n_fc_layers-1) + [self.out_dim])
        self.n_temp_filters = net_params.get('n_temp_filters',[32,32,1])
        self.temp_kernel_size = net_params.get('temp_kernel_size',[10,10,1])
        self.seed = net_params.get('seed',0)
        self.init_data()
        self.init_graph()
        
    def init_data(self):
        self.Xi = tf.placeholder(shape = [None,self.img_width,self.img_height,3],dtype=tf.float32)
        self.state = tf.placeholder(shape=[None,self.state_dim],dtype=tf.float32)
        self.actions = tf.placeholder(shape=[None,self.out_dim],dtype=tf.float32)
        
        self.conv_out = self.Xi
        
    def conv_layers(self):
        for i in range(self.n_conv_layers):
            self.conv_out = tf.contrib.layers.convolution(self.conv_out,\
                            self.n_filters[i],self.kernel_size[i],stride=self.stride[i], \
                            weights_initializer=tf.initializers.random_uniform(minval=-0.08,maxval=0.08,\
                            seed=self.seed))
    
    
    def spatial_softmax(self):
        self.features2D = tf.contrib.layers.spatial_softmax(self.conv_out)
        
    
    def fc_layers(self):
        
        tf.fc = tf.concat([self.features2D,self.state],axis=1)
        for i in range(self.n_fc_layers - 1):
            self.fc = tf.contrib.layers.fully_connected(self.fc,self.hidden_dim[i],\
                                    weights_initializer=tf.initializers.random_uniform(minval=-0.08,maxval=0.08,\
                                    seed=self.seed))
            
        self.predicted_actions = tf.contrib.layers.fully_connected(self.fc,self.hidden_dim[i],\
                                    weights_initializer=tf.initializers.random_uniform(minval=-0.08,maxval=0.08,\
                                    seed=self.seed))
    
    
    def temporal_loss(self):
        self.temp = tf.concat([self.features2D,self.fc],axis = 1)
        for i in range(self.n_temp_layers):
            self.temp = tf.contrib.layers.conv1d(self.temp,n_temp_filters[i],\
                                    self.temp_kernel_size[i],\
                                    weights_initializer=tf.initializers.random_uniform(minval=-0.08,maxval=0.08,\
                                    seed=self.seed))
        self.loss_shi = self.temp
        
    def bc_loss(self):
        l1_loss = tf.losses.absolute_difference(self.actions,self.predicted_actions)
        l2_loss = tf.losses.mean_squared_error(self.actions,self.predicted_actions)
        
        self.bc_loss = l1_loss + (0.001*l2_loss)
        
    def init_inner_optimizer(self):
        inner_optimizer = tf.train.AdamOptimizer(1e-3)
        gradients, variables = zip(*inner_optimizer.compute_gradients(self.loss_shi))
        gradients, _ = tf.clip_by_global_norm(gradients, 5.0)
        self.inner_trainop = inner_optimizer.apply_gradients(zip(gradients, variables))
        
    def init_outer_optimizer(self):
        outer_optimizer = tf.train.AdamOptimizer(1e-3)
        gradients, variables = zip(*outer_optimizer.compute_gradients(self.bc_loss))
        gradients, _ = tf.clip_by_global_norm(gradients, 5.0)
        self.outer_trainop = outer_optimizer.apply_gradients(zip(gradients, variables))
    
    def init_graph(self):
        self.conv_layers()
        self.spatial_softmax()
        self.fc_layers()
        self.temporal_loss()
        self.bc_loss()
        self.init_inner_optimizer()
        self.init_outer_optimizer()
        
        
    def inner_update(sess,obs,state):
        loss,_ = sess.run([self.loss_shi,self.inner_trainop],feed_dict={self.Xi:obs,self.state:state})
        return loss
    
    def predict(sess,obs,state):
        actions = sess.run(self.predicted_actions,feed_dict={self.Xi:obs,self.state:state})
        return actions
    
    def outer_update(sess,obs,state,actions):
        loss,_ = sess.run([self.bc_loss,self.outer_trainop],\
                          feed_dict={self.Xi:obs,self.state:state,self.actions:actions})
        
        return loss