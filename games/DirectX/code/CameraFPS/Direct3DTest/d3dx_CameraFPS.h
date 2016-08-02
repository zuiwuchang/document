//CameraFPS.h	一個 第一人稱 攝像機
#ifndef _DARK_D3DX_CAMERA_FPS_
#define _DARK_D3DX_CAMERA_FPS_


namespace d3dx
{
	class CameraFPS
	{
	public:
		CameraFPS(void);
		~CameraFPS(void);

		inline void Create(IDirect3DDevice9* device_ptr)
		{
			device_ptr_	=	device_ptr;
		}
	private:
		IDirect3DDevice9*	device_ptr_;

		//攝像機朝向
		D3DXVECTOR3 up_;
		D3DXVECTOR3 front_;
		D3DXVECTOR3 right_;
		
		//攝像機位置
		D3DXVECTOR3 position_;

	public:
		//計算 投影矩陣
		void ComputeViewMatrix(D3DXMATRIX* matrix_ptr);
		inline void Refresh()
		{
			D3DXMATRIX matrix;
			ComputeViewMatrix(&matrix);
			device_ptr_->SetTransform(D3DTS_VIEW,&matrix);
		}
		inline void Refresh(D3DXMATRIX& matrix)
		{
			device_ptr_->SetTransform(D3DTS_VIEW,&matrix);
		}


		//設置攝像機位置
		inline void Position(const D3DXVECTOR3& position)
		{
			position_	=	position;
		}
		inline D3DXVECTOR3 Position() const 
		{
			return position_;
		}

		inline void Up(const D3DXVECTOR3& up)
		{
			up_	=	up;
		}
		inline D3DXVECTOR3 Up() const 
		{
			return up_;
		}
		inline void Front(const D3DXVECTOR3& front)
		{
			front_	=	front;
		}
		inline D3DXVECTOR3 Front() const 
		{
			return front_;
		}
		inline void Right(const D3DXVECTOR3& right)
		{
			right_	=	right;
		}
		inline D3DXVECTOR3 Right() const 
		{
			return right_;
		}

		//旋轉
		void RotateUp(const float angle);		//偏航
		void RotateRight(float angle);			//俯/仰視
		void RotateFront(const float angle);	//滾動

		inline void MoveFront(const float step)		//行走
		{
			//限制在水平面上(height) 移動
			//position_ += D3DXVECTOR3(front_.x, height, front_.z) * step;

			position_ += front_ * step;
		}

		inline void MoveRight(const float step)	//掃視
		{
			position_ += right_ * step;
		}

		inline void MoveUp(const float step)	//升降
		{
			//對於fps視角 可能發生了 俯/仰 
			//但依然應該 只改變 y坐標
			position_.y	+=	step;
		}
	};
};
#endif //_DARK_D3DX_CAMERA_FPS_